#include "imageproc.h"
#include <QDebug>
#include <QImage>
#include <QFileInfo>
#include <QDir>
#include <QDesktopServices>

namespace {
enum code{
    success=0,
    fileLoadError,
    zeroBitCount,
    imageLoadError,
};
}

ImageProc::ImageProc(QObject *parent) : QObject(parent)
{

}

int ImageProc::hideImage()
{
    qDebug() << m_originalImageUrl.toString();
    qDebug() << m_payloadImageUrl.toString();

    if(!m_originalImageUrl.isLocalFile() || !m_payloadImageUrl.isLocalFile()) {
        return code::fileLoadError;
    }

    if(m_bitCount < 1) {
        return code::zeroBitCount;
    }

    const QImage originalImage = QImage(m_originalImageUrl.url(QUrl::PreferLocalFile));
    if(originalImage.isNull()) return code::imageLoadError;
    const QImage payloadImage = QImage(m_payloadImageUrl.url(QUrl::PreferLocalFile));
    if(payloadImage.isNull()) return code::imageLoadError;

    /* resize payloadImage to fit in carrier
     */
    const QImage scaledPayloadImage = payloadImage.scaled(originalImage.width(), originalImage.height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    //    scaledPayloadImage.save("/home/taskmaster9001/0.jpg"); // scaled

    QImage bnwPayloadImage(scaledPayloadImage.size(), QImage::Format_Indexed8);

    /* 8-bit depth images use a index values in place of pixels and the
     * actual 8-bit color values are looked up in a color table supplied
     * with the image. For our usecase we just map each value in
     * color table to the index itself so we get 0 - 255 values.
     * Each entry in the color table is 32-bit though.
     */
    for (int i = 0; i <= 255; i++) {
        bnwPayloadImage.setColor(i,QColor(quint8(i), quint8(i), quint8(i)).rgba());
    }

    for(int i = 0; i < bnwPayloadImage.height(); i++) {
        const uchar* scan = scaledPayloadImage.constScanLine(i);
        /* I really don't think I should be doing scanLine in the next line.
             * please let me know if you know what needs to be done. :D
             */
        uchar* bnwScan = bnwPayloadImage.scanLine(i);
        for (int j =0; j < bnwPayloadImage.width(); j++) {
            const QRgb* rgbPixel = reinterpret_cast<const QRgb*>(
                        scan + j * (scaledPayloadImage.depth() / 8));
            QRgb* bnwPixel = reinterpret_cast<QRgb*>(bnwScan+j);
            quint8 gray = qGray(*rgbPixel);
            *bnwPixel = gray;
            if( i == 0 && j == 0) {
                qDebug() << "*bnwPixel = " << QString::number(*bnwPixel, 2);
            }

        }
    }

    /* Now we apply the logic for steganography!
     */
    QImage resultImage(bnwPayloadImage.size(), QImage::Format_RGB32);

    for (int i = 0; i < resultImage.height(); i++) {
        const uchar* carrierScan = originalImage.constScanLine(i);
        const uchar* payloadScan = bnwPayloadImage.constScanLine(i);
        uchar* resultScan = resultImage.scanLine(i);

        for ( int j = 0; j < resultImage.width(); j++) {
            const QRgb* carrierPixel = reinterpret_cast<const QRgb*>(
                        carrierScan + j * (originalImage.depth() / 8));
            const QRgb* payloadPixel = reinterpret_cast<const QRgb*>(payloadScan + j);
            QRgb* resultPixel = reinterpret_cast<QRgb*>(
                        resultScan + j * (resultImage.depth() / 8));

            const quint8 payloadPixelInt = (*payloadPixel);

            if( i == 0 && j == 0) {
                qDebug() << "payloadPixelInt = " << QString::number(payloadPixelInt, 2);
            }

            quint8 resultGreen = qGreen(*carrierPixel);
            quint8 resultRed = qRed(*carrierPixel);
            quint8 resultBlue = qBlue(*carrierPixel);

            if( i == 0 && j == 0) {
                qDebug() << "carrierGreen = " << QString::number(resultGreen, 2);
                qDebug() << "carrierRed = " << QString::number(resultRed, 2);
                qDebug() << "carrierBlue = " << QString::number(resultBlue, 2);
            }

            /* the logic is a bit novel; it takes into account
             * the sensitivity of human eyes towards the color Green.
             * Consequently, much care has been given to the fact that
             * green pixel values should be the least manipulated ones.
             * To ensure this, 2 MSBs from the payload are hidden in the
             * carrier pixel's green values' 2 LSBs.
             */

            // greenSlice will have 11000000 bits from payloadPixel as its LSBs
            quint8 greenSlice = (payloadPixelInt & 0b11000000) >> (8- ((m_bitCount > 2) ? 2 : 1));

            // redSlice will have 00111000 bits from payloadPixel as its LSBs
            quint8 redSlice = (payloadPixelInt & 0b00111000) >> (6-m_bitCount);

            // blueSlice will have 00000111 bits from payloadPixel as its LSBs
            quint8 blueSlice = (payloadPixelInt & 0b00000111) >> (3-m_bitCount);

            if( i == 0 && j == 0) {
                // these should have the LSBs populated
                qDebug() << "greenSlice = " << QString::number(greenSlice, 2);
                qDebug() << "redSlice = " << QString::number(redSlice, 2);
                qDebug() << "blueSlice = " << QString::number(blueSlice, 2);
            }

            /* apply the masks and prepare the pixel's bytes
             */
            resultGreen = (resultGreen & (0xff << ((m_bitCount > 2) ? 2 : 1)))      // wipe result pixel bits
                    | greenSlice;                                                   // apply mask to store payload bits

            resultRed = (resultRed & (0xff << m_bitCount ))
                    | redSlice;

            resultBlue= (resultBlue& (0xff << m_bitCount ))
                    | blueSlice;

            if( i == 0 && j == 0) {
                // these should be ready to become modulated pixel
                qDebug() << "resultGreen= " << QString::number(resultGreen, 2);
                qDebug() << "resultRed= " << QString::number(resultRed, 2);
                qDebug() << "resultBlue= " << QString::number(resultBlue, 2);
            }

            *resultPixel = QColor(resultRed, resultGreen, resultBlue).rgba();
        }
    }

    tempResultFile.setFileTemplate((QDir::tempPath()+"/XXXXXX.png"));
    tempResultFile.open();
    tempResultFile.close();

    QFileInfo tempResultFileInfo(tempResultFile);
    resultImage.save(tempResultFileInfo.absoluteFilePath());

    QUrl tempResultFileUrl = QUrl(tempResultFile.fileName());
    tempResultFileUrl.setScheme("file");
    m_resultImageUrl = tempResultFileUrl;

    emit resultImageUrlChanged();

    qDebug() << "result = " << tempResultFileUrl;

    return code::success;
}


int ImageProc::retrieveImage()
{
    if(!m_resultImageUrl.isLocalFile()) {
        return code::fileLoadError;
    }

    if(m_bitCount < 1) {
        return code::zeroBitCount;
    }

    QImage modulatedImage = QImage(m_resultImageUrl.url(QUrl::PreferLocalFile));

    if(modulatedImage.isNull()) {
        return code::imageLoadError;
    }

    QImage hiddenImage(modulatedImage.size(), QImage::Format_Indexed8);
    QImage carrierImage(modulatedImage.size(), QImage::Format_RGB32);

    /* populate color table for hiddenImage
     */
    for (int i = 0; i <= 255; i++) {
        hiddenImage.setColor(i,QColor(quint8(i), quint8(i), quint8(i)).rgba());
    }

    /* TODO: make it modular enough to use any algo
     */
    for (int i = 0; i < hiddenImage.height(); i++) {
        const uchar* modulatedImageScan = modulatedImage.constScanLine(i);
        uchar* hiddenImageScan = hiddenImage.scanLine(i);
        uchar* carrierImageScan = carrierImage.scanLine(i);

        for (int j = 0; j<hiddenImage.width(); j++) {
            const QRgb* modulatedImageRgbPixel = reinterpret_cast<const QRgb*>(modulatedImageScan + j * (modulatedImage.depth() / 8));
            QRgb* hiddenImageRgbPixel = reinterpret_cast<QRgb*>(hiddenImageScan + j * (hiddenImage.depth() / 8));
            QRgb* carrierImageRgbPixel = reinterpret_cast<QRgb*>(carrierImageScan + j * (carrierImage.depth() / 8));

            quint8 hiddenImagePixel = 0;
            quint8 modulatedGreen = qGreen(*modulatedImageRgbPixel);
            quint8 modulatedRed = qRed(*modulatedImageRgbPixel);
            quint8 modulatedBlue = qBlue(*modulatedImageRgbPixel);

            if( i == 0 && j == 0) {
                // these should be ready to become modulated pixel
                qDebug() << "modulatedGreen = " << QString::number(modulatedGreen, 2);
                qDebug() << "modulatedRed = " << QString::number(modulatedRed, 2);
                qDebug() << "modulatedBlue = " << QString::number(modulatedBlue, 2);
            }

            /* here we retrieve the bits of the carrier and hidden images from the modulated image
             */

            quint8 greenSlice;
            quint8 redSlice;
            quint8 blueSlice;

            /* extract the hidden image
             */
            greenSlice = modulatedGreen &
                    (~(0xff << ((m_bitCount > 2) ? 2 : 1)));
            redSlice = modulatedRed &
                    (~(0xff << m_bitCount));
            blueSlice = modulatedBlue &
                    (~(0xff << m_bitCount));

            if( i == 0 && j == 0) {
                qDebug() << "greenSlice = " << QString::number(greenSlice, 2);
                qDebug() << "redSlice = " << QString::number(redSlice, 2);
                qDebug() << "blueSlice = " << QString::number(blueSlice, 2);
            }

            greenSlice = greenSlice << (8 - ((m_bitCount > 2) ? 2 : 1));
            redSlice = redSlice << (6 - m_bitCount);
            blueSlice = blueSlice << (3 - m_bitCount);

            hiddenImagePixel = hiddenImagePixel | greenSlice | redSlice | blueSlice;
            *hiddenImageRgbPixel = hiddenImagePixel;

            /* recover the carrier image
             */
            greenSlice = modulatedGreen &
                    ((0xff << ((m_bitCount > 2) ? 2 : 1)));
            redSlice = modulatedRed &
                    ((0xff << m_bitCount));
            blueSlice = modulatedBlue &
                    ((0xff << m_bitCount));

            if( i == 0 && j == 0) {
                qDebug() << "greenSlice = " << QString::number(greenSlice, 2);
                qDebug() << "redSlice = " << QString::number(redSlice, 2);
                qDebug() << "blueSlice = " << QString::number(blueSlice, 2);
            }
            *carrierImageRgbPixel = QColor(redSlice, greenSlice, blueSlice).rgba();
        }
    }

    //    hiddenImage.save("/home/taskmaster9001/hiddenImage.png");

    //    carrierImage.save("/home/taskmaster9001/carrierImage.png");

    tempHiddenFile.setFileTemplate(QDir::tempPath() + "/XXXXXXXXXXXX.png");
    tempHiddenFile.open();
    tempHiddenFile.close();

    tempCarrierFile.setFileTemplate(QDir::tempPath() + "/XXXXXXXXXXXX.png");
    tempCarrierFile.open();
    tempHiddenFile.close();

    QFileInfo tempHiddenFileInfo(tempHiddenFile);
    hiddenImage.save(tempHiddenFileInfo.absoluteFilePath());

    QFileInfo tempCarrierFileInfo(tempCarrierFile);
    carrierImage.save(tempCarrierFileInfo.absoluteFilePath());

    QUrl tempHiddenFileUrl(tempHiddenFile.fileName());
    tempHiddenFileUrl.setScheme("file");

    QUrl tempCarrierFileUrl(tempCarrierFile.fileName());
    tempCarrierFileUrl.setScheme("file");

    m_payloadImageUrl = tempHiddenFileUrl;
    emit payloadImageUrlChanged();

    m_originalImageUrl = tempCarrierFileUrl;
    emit originalImageUrlChanged();

    qDebug() << "carrier file = " << tempCarrierFileUrl;
    qDebug() << "hidden file = " << tempHiddenFileUrl;


    return code::success;
}

QUrl ImageProc::getOriginalImageUrl() const
{
    return m_originalImageUrl;
}

void ImageProc::openImage(const int &n) const
{
    switch(n) {
    case 0 :
        QDesktopServices::openUrl(m_originalImageUrl);
        break;
    case 1 :
        QDesktopServices::openUrl(m_payloadImageUrl);
        break;
    case 2 :
        QDesktopServices::openUrl(m_resultImageUrl);
        break;
    default:
        qDebug() << "invalid input";
        return;
    }
}
