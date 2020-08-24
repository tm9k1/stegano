#include "steganographylogic.h"
#include <QDebug>

SteganographyLogic::SteganographyLogic()
{

}

bool SteganographyLogic::hideImage(QImage const* carrierImage, QImage const* payloadImage, QImage* modulatedImage, const int m_bitCount)
{
    // mem check
    if(carrierImage == nullptr || payloadImage == nullptr || modulatedImage == nullptr) {
        qDebug() << "QImage objects were not provided. Returning control.";
        return false;
    }
    // data check
    if(carrierImage->isNull() || payloadImage->isNull()) {
        qDebug() << "Carrier/Payload image was null. Returning control.";
        return false;
    }
    // param check
    if(m_bitCount < 1 || m_bitCount > 3) {
        qDebug() << "invalid bitCount. Returning control.";
        return false;
    }

    // good to go!

    /* resize payloadImage to fit in carrier
     */
    const QImage* scaledPayloadImage = new QImage(payloadImage->scaled(carrierImage->width(), carrierImage->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    /* convert payload to grayscale
     * 8-bit depth images use a index values in place of pixels and the
     * actual 8-bit color values are looked up in a color table supplied
     * with the image. For our usecase we just map each value in
     * color table to the index itself so we get 0 - 255 values.
     * Each entry in the color table is 32-bit though.
     */
    QImage* bnwPayloadImage = new QImage(scaledPayloadImage->size(), QImage::Format_Indexed8);

    // populate the color table
    for (int i = 0; i <= 255; i++) {
        bnwPayloadImage->setColor(i,QColor(quint8(i), quint8(i), quint8(i)).rgba());
    }
    // populate the pixels
    for(int i = 0; i < bnwPayloadImage->height(); i++) {
        const uchar* scan = scaledPayloadImage->constScanLine(i);
        /* I really don't think I should be doing scanLine in the next line.
             * please let me know if you know what needs to be done. :D
             */
        uchar* bnwPayloadScan = bnwPayloadImage->scanLine(i);
        for (int j = 0; j < bnwPayloadImage->width(); j++) {
            const QRgb* scaledPayloadPixel = reinterpret_cast<const QRgb*>(
                        scan + j * (scaledPayloadImage->depth() / 8));
            quint8* bnwPixel = reinterpret_cast<quint8*>(
                        bnwPayloadScan + j * (bnwPayloadImage->depth() / 8));

            *bnwPixel = qGray(*scaledPayloadPixel);
            if( i == 0 && j == 0) {
                qDebug() << "*bnwPixel = " << QString::number(*bnwPixel, 2);
            }
        }
    }

    /* the logic takes into account
     * the sensitivity of human eyes towards the color Green.
     * Consequently, green pixel values should be the least manipulated ones.
     * To ensure this, 2 MSBs from the payload are hidden in the
     * carrier pixel's green values' 2 LSBs.
     * Remaining pixels are currently filled with 0, i.e. the
     * hidden image has black color filled for any remaining pixels
     */
    for (int i = 0; i < modulatedImage->height(); i++) {
        const uchar* carrierScan = carrierImage->constScanLine(i);

        const uchar* bnwPayloadImageScan = (i < bnwPayloadImage->height()) ?
                    bnwPayloadImage->constScanLine(i) : nullptr;

        uchar* modulatedImageScan = modulatedImage->scanLine(i);

        for ( int j = 0; j < modulatedImage->width(); j++) {
            const QRgb* carrierPixel = reinterpret_cast<const QRgb*>(
                        carrierScan + j * (carrierImage->depth() / 8));
            quint8 carrierGreen = qGreen(*carrierPixel);
            quint8 carrierRed = qRed(*carrierPixel);
            quint8 carrierBlue = qBlue(*carrierPixel);

            if( i == 0 && j == 0) {
                qDebug() << "carrierGreen = " << QString::number(carrierGreen, 2);
                qDebug() << "carrierRed   = " << QString::number(carrierRed, 2);
                qDebug() << "carrierBlue  = " << QString::number(carrierBlue, 2);
            }

            // these should have the LSBs wiped clean
            quint8 modulatedGreenComponent = (carrierGreen & (0xff << ((m_bitCount > 2) ? 2 : 1)));
            quint8 modulatedRedComponent = (carrierRed & (0xff << m_bitCount ));
            quint8 modulatedBlueComponent = (carrierBlue& (0xff << m_bitCount ));

            if( i == 0 && j == 0) {
                qDebug() << "init modulatedGreenComponent = " << QString::number(modulatedGreenComponent, 2);
                qDebug() << "init modulatedRedComponent   = " << QString::number(modulatedRedComponent, 2);
                qDebug() << "init modulatedBlueComponent  = " << QString::number(modulatedBlueComponent, 2);
            }

            if (j < bnwPayloadImage->width() && bnwPayloadImageScan != nullptr) {
                const quint8* payloadImagePixel = reinterpret_cast<const quint8*>(
                            bnwPayloadImageScan + j);

                //                if( i == 0 && j == 0) {
                //                    qDebug() << "payloadPixelInt = " << QString::number(*payloadImagePixel, 2);
                //                }

                // these should be masked from payloadImagePixel
                quint8 greenComponentMask = ((*payloadImagePixel) & 0b11000000);
                quint8 redComponentMask = ((*payloadImagePixel) & 0b00111000);
                quint8 blueComponentMask = ((*payloadImagePixel) & 0b00000111);

                //                if( i == 0 && j == 0) {
                //                    qDebug() << "Bit mask for green component = " << QString::number(greenComponentMask, 2);
                //                    qDebug() << "Bit mask for red component   = " << QString::number(redComponentMask, 2);
                //                    qDebug() << "Bit mask for blue component  = " << QString::number(blueComponentMask, 2);
                //                }

                // these should be modulated RGB data values
                modulatedGreenComponent = modulatedGreenComponent | (greenComponentMask >> (8- ((m_bitCount > 2) ? 2 : 1)));
                modulatedRedComponent = modulatedRedComponent | (redComponentMask >> (6-m_bitCount));
                modulatedBlueComponent = modulatedBlueComponent | (blueComponentMask >> (3-m_bitCount));
            }

            if( i == 0 && j == 0) {
                // these should be ready to become modulated pixel
                qDebug() << "modulatedGreenComponent = " << QString::number(modulatedGreenComponent, 2);
                qDebug() << "modulatedRedComponent   = " << QString::number(modulatedRedComponent, 2);
                qDebug() << "modulatedBlueComponent  = " << QString::number(modulatedBlueComponent, 2);
            }

            QRgb* modulatedImagePixel = reinterpret_cast<QRgb*>(
                        modulatedImageScan + j * (modulatedImage->depth() / 8));
            *modulatedImagePixel = QColor(modulatedRedComponent, modulatedGreenComponent, modulatedBlueComponent).rgba();
        }
    }
    delete bnwPayloadImage;
    delete scaledPayloadImage;
    return true;
}

bool SteganographyLogic::retrieveImages(QImage* carrierImage, QImage* hiddenImage, const QImage* modulatedImage, const int m_bitCount)
{
    // mem check
    if(carrierImage == nullptr || hiddenImage == nullptr || modulatedImage == nullptr) {
        qDebug() << "QImage objects were not provided. Returning control.";
        return false;
    }
    // data check
    if(modulatedImage->isNull()) {
        qDebug() << "Modulated image was null. Returning control.";
        return false;
    }
    // param check
    if(m_bitCount < 1 || m_bitCount > 3) {
        qDebug() << "invalid bitCount. Returning control.";
        return false;
    }

    // good to go!

    /* populate color table for hiddenImage
     */
    for (int i = 0; i <= 255; i++) {
        hiddenImage->setColor(i,QColor(quint8(i), quint8(i), quint8(i)).rgba());
    }

    for (int i = 0; i < modulatedImage->height(); i++) {
        const uchar* modulatedImageScan = modulatedImage->constScanLine(i);
        uchar* hiddenImageScan = hiddenImage->scanLine(i);
        uchar* carrierImageScan = carrierImage->scanLine(i);

        for (int j = 0; j<modulatedImage->width(); j++) {
            const QRgb* modulatedImageRgbPixel = reinterpret_cast<const QRgb*>(modulatedImageScan + j * (modulatedImage->depth() / 8));

            const quint8 modulatedImageGreen = qGreen(*modulatedImageRgbPixel);
            const quint8 modulatedImageRed = qRed(*modulatedImageRgbPixel);
            const quint8 modulatedImageBlue = qBlue(*modulatedImageRgbPixel);

            if( i == 0 && j == 0) {
                // these should be ready to become modulated pixel
                qDebug() << "modulatedImageGreen = " << QString::number(modulatedImageGreen, 2);
                qDebug() << "modulatedImageRed = " << QString::number(modulatedImageRed, 2);
                qDebug() << "modulatedImageBlue = " << QString::number(modulatedImageBlue, 2);
            }

            /* here we extract the bits of the carrier and hidden image from the modulated image
             */
            quint8 greenComponentMask;
            quint8 redComponentMask;
            quint8 blueComponentMask;

            /* extract the hidden image
             */
            greenComponentMask = modulatedImageGreen &
                    (~(0xff << ((m_bitCount > 2) ? 2 : 1)));
            redComponentMask = modulatedImageRed &
                    (~(0xff << m_bitCount));
            blueComponentMask = modulatedImageBlue &
                    (~(0xff << m_bitCount));

            greenComponentMask = greenComponentMask << (8 - ((m_bitCount > 2) ? 2 : 1));
            redComponentMask = redComponentMask << (6 - m_bitCount);
            blueComponentMask = blueComponentMask << (3 - m_bitCount);

            quint8* hiddenImagePixel = reinterpret_cast<quint8*>(hiddenImageScan + j * (hiddenImage->depth() / 8));
            *hiddenImagePixel = greenComponentMask | redComponentMask | blueComponentMask; // 11|111|111

            if( i == 0 && j == 0) {
                qDebug() << "greenComponentMask for Hidden Image = " << QString::number(greenComponentMask, 2);
                qDebug() << "redComponentMask for Hidden Image   = " << QString::number(redComponentMask, 2);
                qDebug() << "blueComponentMask for Hidden Image  = " << QString::number(blueComponentMask, 2);
            }

            /* recover the carrier image
             */
            greenComponentMask = modulatedImageGreen &
                    ((0xff << ((m_bitCount > 2) ? 2 : 1)));
            redComponentMask = modulatedImageRed &
                    ((0xff << m_bitCount));
            blueComponentMask = modulatedImageBlue &
                    ((0xff << m_bitCount));

            QRgb* carrierImagePixel = reinterpret_cast<QRgb*>(carrierImageScan + j * (carrierImage->depth() / 8));
            *carrierImagePixel = QColor(redComponentMask, greenComponentMask, blueComponentMask).rgba();

            if( i == 0 && j == 0) {
                qDebug() << "greenComponentMask for Carrier Image = " << QString::number(greenComponentMask, 2);
                qDebug() << "redComponentMask for Carrier Image   = " << QString::number(redComponentMask, 2);
                qDebug() << "blueComponentMask for Carrier Image  = " << QString::number(blueComponentMask, 2);
            }
        }
    }
    return true;
}
