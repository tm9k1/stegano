#include "imageproc.h"

#include <QDebug>
#include <QImage>
#include <QFileInfo>
#include <QDir>
#include <QDesktopServices>

#include "steganographylogic.h"
#include "util.h"

ImageProc* ImageProc::m_instance = nullptr;

ImageProc::ImageProc(QObject *parent) : QObject(parent), m_tempModulatedImageFile(nullptr)
{
}

ImageProc* ImageProc::getInstance()
{
    if (ImageProc::m_instance == nullptr) {
        ImageProc::m_instance = new ImageProc();
    }
    return ImageProc::m_instance;
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
        return Util::ReturnCode::FileLoadError;
    }

    // check if inputs were valid
    if (m_bitCount < Util::minimumBitCount || m_bitCount > Util::maximumBitCount) {
        return Util::ReturnCode::InvalidParam;
    }

    QImage carrierImage     (m_carrierImageUrl.url(QUrl::PreferLocalFile));
    QImage payloadImage     (m_payloadImageUrl.url(QUrl::PreferLocalFile));
    QImage modulatedImage   (carrierImage.size(), QImage::Format_RGB32);

    if (carrierImage.isNull() || payloadImage.isNull() || modulatedImage.isNull()) {
        return Util::ReturnCode::ImageLoadError;
    }

    if (SteganographyLogic::hideImage(carrierImage, payloadImage, modulatedImage,m_bitCount) != true) {
        return Util::ReturnCode::ImageProcessError;
    }

    resetTempFile(m_tempModulatedImageFile);

    QFileInfo tempModulatedImageFileInfo(*m_tempModulatedImageFile);

    bool modulatedImageSaveSuccess = modulatedImage.save(tempModulatedImageFileInfo.absoluteFilePath(),
                                                        Util::imageFormat.toStdString().c_str());

    if (modulatedImageSaveSuccess) {
        QUrl tempModulatedImageUrl = QUrl::fromLocalFile(tempModulatedImageFileInfo.absoluteFilePath());
        m_modulatedImageUrl = tempModulatedImageUrl;
        emit modulatedImageUrlChanged();
        qDebug() << "modulatedImage is here:" << tempModulatedImageUrl;
        return Util::ReturnCode::Success;
    }

    return Util::ReturnCode::ImageProcessError;
}

int ImageProc::retrieveImage()
{
    if (!m_modulatedImageUrl.isLocalFile()) {
        return Util::ReturnCode::FileLoadError;
    }
    if (m_bitCount < Util::minimumBitCount || m_bitCount > Util::maximumBitCount) {
        return Util::ReturnCode::InvalidParam;
    }

    QImage modulatedImage   (m_modulatedImageUrl.url(QUrl::PreferLocalFile));
    QImage carrierImage     (modulatedImage.size(), QImage::Format_RGB32);
    QImage payloadImage     (modulatedImage.size(), QImage::Format_Indexed8);

    if (modulatedImage.isNull() || carrierImage.isNull() || payloadImage.isNull()) {
        return Util::ReturnCode::ImageLoadError;
    }

    if (SteganographyLogic::retrieveImages(carrierImage, payloadImage, modulatedImage, m_bitCount)  != true) {
        return Util::ReturnCode::ImageProcessError;
    }

    resetTempFile(m_tempCarrierImageFile);
    QFileInfo tempCarrierFileInfo(*m_tempCarrierImageFile);

    bool carrierImageSaveSuccess = carrierImage.save(
                tempCarrierFileInfo.absoluteFilePath(),
                Util::imageFormat.toStdString().c_str());

    if (carrierImageSaveSuccess) {
        QUrl tempCarrierFileUrl = QUrl::fromLocalFile(tempCarrierFileInfo.absoluteFilePath());
        m_carrierImageUrl = tempCarrierFileUrl;
        emit carrierImageUrlChanged();
        qDebug() << "Carrier file = " << tempCarrierFileUrl;
    }

    resetTempFile(m_tempPayloadImageFile);
    QFileInfo tempPayloadFileInfo(*m_tempPayloadImageFile);

    bool payloadImageSaveSuccess = payloadImage.save(
                tempPayloadFileInfo.absoluteFilePath(),
                Util::imageFormat.toStdString().c_str());

    if (payloadImageSaveSuccess) {
        QUrl tempPayloadFileUrl = QUrl::fromLocalFile(tempPayloadFileInfo.absoluteFilePath());
        m_payloadImageUrl = tempPayloadFileUrl;
        emit payloadImageUrlChanged();
        qDebug() << "Payload file = " << tempPayloadFileUrl;
    }

    if (carrierImageSaveSuccess && payloadImageSaveSuccess) {
        return Util::ReturnCode::Success;
    } else {
        if (!carrierImageSaveSuccess) {
            qDebug() << "Carrier Image could not be retrieved due to error while saving the image.";
        }
        if (!payloadImageSaveSuccess) {
            qDebug() << "Payload Image could not be retrieved due to error while saving the image.";
        }
    }
    return Util::ReturnCode::ImageProcessError;

}

void ImageProc::resetTempFile(QPointer<QTemporaryFile> &tempFile)
{
    if (tempFile != nullptr) { delete tempFile; }

    tempFile = new QTemporaryFile();
    tempFile->setFileTemplate((QDir::tempPath()+QStringLiteral("/XXXXXXXXXX")+
                               QStringLiteral(".")+Util::imageFormat));

    // open and close the file once so that tempFile.fileName() is populated
    tempFile->open();
    tempFile->close();
}

bool ImageProc::openImage(const QUrl &url) const
{
    bool result = QDesktopServices::openUrl(url);

    if (result == true) {
        return Util::ReturnCode::Success;
    }

    return Util::ReturnCode::FileIOError;
}

bool ImageProc::saveImage(const QUrl &sourceUrl, const QUrl &destinationUrl) const
{
    bool result = QFile::copy(sourceUrl.url(QUrl::PreferLocalFile), destinationUrl.url(QUrl::PreferLocalFile));

    if (result == true) {
        return Util::ReturnCode::Success;
    }

    return Util::ReturnCode::FileIOError;
}
