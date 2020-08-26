#include "imageproc.h"
#include <QDebug>
#include <QImage>
#include <QFileInfo>
#include <QDir>
#include <QDesktopServices>

#include "steganographylogic.h"

ImageProc::ImageProc(QObject *parent) : QObject(parent), m_tempModulatedImageFile(nullptr)
{
}

ImageProc::~ImageProc()
{
    if (m_tempCarrierImageFile != nullptr) { delete m_tempCarrierImageFile; }
    if (m_tempPayloadImageFile != nullptr) { delete m_tempPayloadImageFile; }
    if (m_tempModulatedImageFile != nullptr) { delete m_tempModulatedImageFile; }
}

int ImageProc::hideImage()
{
    // check if file exists
    if (!m_carrierImageUrl.isLocalFile() || !m_payloadImageUrl.isLocalFile()) {
        return ImageProcUtil::ReturnCode::FileLoadError;
    }

    // check if inputs were valid
    if (m_bitCount < ImageProcUtil::minimumBitCount || m_bitCount > ImageProcUtil::maximumBitCount) {
        return ImageProcUtil::ReturnCode::InvalidBitCount;
    }

    const QImage* carrierImage = new QImage(
                m_carrierImageUrl.url(QUrl::PreferLocalFile));
    if (carrierImage == nullptr || carrierImage->isNull()) {
        return ImageProcUtil::ReturnCode::ImageLoadError;
    }

    const QImage* payloadImage = new QImage(
                m_payloadImageUrl.url(QUrl::PreferLocalFile));
    if (payloadImage  == nullptr || payloadImage->isNull()) {
        return ImageProcUtil::ReturnCode::ImageLoadError;
    }

    QImage* modulatedImage = new QImage(
                carrierImage->size(), QImage::Format_RGB32);
    if (modulatedImage == nullptr || modulatedImage->isNull()) {
        return ImageProcUtil::ReturnCode::ImageLoadError;
    }

    if (SteganographyLogic::hideImage(
                carrierImage, payloadImage, modulatedImage,m_bitCount) != true) {
        return ImageProcUtil::ReturnCode::ImageProcessError;
    }

    delete carrierImage;
    delete payloadImage;

    resetTempFile(m_tempModulatedImageFile);

    QFileInfo tempModulatedImageFileInfo(*m_tempModulatedImageFile);

    bool modulatedImageSaveSuccess = modulatedImage->save(
                tempModulatedImageFileInfo.absoluteFilePath(),
                ImageProcUtil::imageFormat.toStdString().c_str());

    delete modulatedImage;

    if (modulatedImageSaveSuccess) {
        QUrl tempModulatedImageUrl = QUrl(tempModulatedImageFileInfo.absoluteFilePath());
        tempModulatedImageUrl.setScheme("file");
        m_modulatedImageUrl = tempModulatedImageUrl;

        emit modulatedImageUrlChanged();

        qDebug() << "modulatedImage is here:" << tempModulatedImageUrl;
        return ImageProcUtil::ReturnCode::Success;
    }

    return ImageProcUtil::ReturnCode::ImageProcessError;
}

int ImageProc::retrieveImage()
{
    if (!m_modulatedImageUrl.isLocalFile()) {
        return ImageProcUtil::ReturnCode::FileLoadError;
    }
    if (m_bitCount < ImageProcUtil::minimumBitCount || m_bitCount > ImageProcUtil::maximumBitCount) {
        return ImageProcUtil::ReturnCode::InvalidBitCount;
    }

    const QImage* modulatedImage = new QImage(m_modulatedImageUrl.url(QUrl::PreferLocalFile));
    if (modulatedImage->isNull()) { return ImageProcUtil::ReturnCode::ImageLoadError; }
    QImage* carrierImage = new QImage(modulatedImage->size(), QImage::Format_RGB32);
    if (carrierImage->isNull()) { return ImageProcUtil::ReturnCode::ImageLoadError; }
    QImage* payloadImage = new QImage(modulatedImage->size(), QImage::Format_Indexed8);
    if (payloadImage->isNull()) { return ImageProcUtil::ReturnCode::ImageLoadError; }

    if (SteganographyLogic::retrieveImages(carrierImage, payloadImage, modulatedImage, m_bitCount)  != true) {
        return ImageProcUtil::ReturnCode::ImageProcessError;
    }

    delete modulatedImage;

    resetTempFile(m_tempCarrierImageFile);
    QFileInfo tempCarrierFileInfo(*m_tempCarrierImageFile);

    bool carrierImageSaveSuccess = carrierImage->save(
                tempCarrierFileInfo.absoluteFilePath(),
                ImageProcUtil::imageFormat.toStdString().c_str());

    delete carrierImage;

    if (carrierImageSaveSuccess) {
        QUrl tempCarrierFileUrl(tempCarrierFileInfo.absoluteFilePath());
        tempCarrierFileUrl.setScheme("file");
        m_carrierImageUrl = tempCarrierFileUrl;

        emit carrierImageUrlChanged();

        qDebug() << "Carrier file = " << tempCarrierFileUrl;
    }

    resetTempFile(m_tempPayloadImageFile);
    QFileInfo tempPayloadFileInfo(*m_tempPayloadImageFile);

    bool payloadImageSaveSuccess = payloadImage->save(
                tempPayloadFileInfo.absoluteFilePath(),
                ImageProcUtil::imageFormat.toStdString().c_str());

    delete payloadImage;

    if (payloadImageSaveSuccess) {
        QUrl tempPayloadFileUrl(tempPayloadFileInfo.absoluteFilePath());
        tempPayloadFileUrl.setScheme("file");
        m_payloadImageUrl = tempPayloadFileUrl;

        emit payloadImageUrlChanged();

        qDebug() << "Payload file = " << tempPayloadFileUrl;
    }

    if (carrierImageSaveSuccess && payloadImageSaveSuccess) {
        return ImageProcUtil::ReturnCode::Success;
    } else {
        if (!carrierImageSaveSuccess) {
            qDebug() << "Carrier Image could not be retrieved due to error while saving the image.";
        }
        if (!payloadImageSaveSuccess) {
            qDebug() << "Payload Image could not be retrieved due to error while saving the image.";
        }
    }
    return ImageProcUtil::ReturnCode::ImageProcessError;

}

void ImageProc::resetTempFile(QPointer<QTemporaryFile> &tempFile)
{
    if (tempFile != nullptr) { delete tempFile; }

    tempFile = new QTemporaryFile();
    tempFile->setFileTemplate((QDir::tempPath()+QStringLiteral("/XXXXXXXXXX")+
                               QStringLiteral(".")+ImageProcUtil::imageFormat));

    // open and close the file once so that tempFile->fileName() is populated
    tempFile->open();
    tempFile->close();
}

bool ImageProc::openImage(const QUrl &url) const
{
    return QDesktopServices::openUrl(url);
}

int ImageProc::saveImage(const QUrl &sourceUrl, const QUrl &destinationUrl) const
{
    bool result = QFile::copy(sourceUrl.url(QUrl::PreferLocalFile), destinationUrl.url(QUrl::PreferLocalFile));

    if (result == true) {
        return ImageProcUtil::ReturnCode::Success;
    }

    return ImageProcUtil::ReturnCode::ImageProcessError;
}
