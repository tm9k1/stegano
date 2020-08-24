#include "imageproc.h"
#include <QDebug>
#include <QImage>
#include <QFileInfo>
#include <QDir>
#include <QDesktopServices>

#include "steganographylogic.h"

namespace {
enum code{
    success=0,
    fileLoadError,
    zeroBitCount,
    imageLoadError,
    imageProcessError,
};
}

ImageProc::ImageProc(QObject *parent) : QObject(parent), m_tempResultFile(nullptr)
{
}

ImageProc::~ImageProc()
{
    if (m_tempResultFile != nullptr) {
        delete m_tempResultFile;
    }
}

void ImageProc::resetTempFile(QPointer<QTemporaryFile> &tempFile)
{
    if(tempFile != nullptr) {
        delete tempFile;
    }
    tempFile = new QTemporaryFile();
    tempFile->setFileTemplate((QDir::tempPath()+"/XXXXXXXXXX.png"));
    tempFile->open();
    tempFile->close();
}

int ImageProc::hideImage()
{
    if(!m_carrierImageUrl.isLocalFile() || !m_payloadImageUrl.isLocalFile()) {
        return code::fileLoadError;
    }
    if(m_bitCount < 1 || m_bitCount > 3) {
        return code::zeroBitCount;
    }

    const QImage* carrierImage = new QImage(m_carrierImageUrl.url(QUrl::PreferLocalFile));
    if (carrierImage == nullptr || carrierImage->isNull()) { return code::imageLoadError; }

    const QImage* payloadImage = new QImage(m_payloadImageUrl.url(QUrl::PreferLocalFile));
    if (payloadImage  == nullptr || payloadImage->isNull()) { return code::imageLoadError; }

    QImage* modulatedImage = new QImage(carrierImage->size(), QImage::Format_RGB32);
    if (modulatedImage == nullptr || modulatedImage->isNull()) { return code::imageLoadError; }

    bool result = SteganographyLogic::hideImage(carrierImage, payloadImage, modulatedImage, m_bitCount);
    qDebug() << "hideImage() returned " << result;

    resetTempFile(m_tempResultFile);

    QFileInfo tempResultFileInfo(m_tempResultFile->fileName());
    modulatedImage->save(tempResultFileInfo.absoluteFilePath(), QStringLiteral("png").toStdString().c_str());

    QUrl tempResultFileUrl = QUrl(m_tempResultFile->fileName());
    tempResultFileUrl.setScheme("file");
    m_resultImageUrl = tempResultFileUrl;

    emit resultImageUrlChanged();

    qDebug() << "modulatedImage is here: " << tempResultFileUrl;

    delete carrierImage;
    delete payloadImage;
    delete modulatedImage;

    return code::success;
}


int ImageProc::retrieveImage()
{
    if(!m_resultImageUrl.isLocalFile()) {
        return code::fileLoadError;
    }

    if(m_bitCount < 1 || m_bitCount > 3) {
        return code::zeroBitCount;
    }

    const QImage* modulatedImage = new QImage(m_resultImageUrl.url(QUrl::PreferLocalFile));
    if(modulatedImage->isNull()) { return code::imageLoadError; }
    QImage* carrierImage = new QImage(modulatedImage->size(), QImage::Format_RGB32);
    if(carrierImage->isNull()) { return code::imageLoadError; }
    QImage* payloadImage = new QImage(modulatedImage->size(), QImage::Format_Indexed8);
    if(payloadImage->isNull()) { return code::imageLoadError; }

    bool result = SteganographyLogic::retrieveImages(carrierImage, payloadImage, modulatedImage, m_bitCount);
    qDebug() << "retrieveImage() returned " << result;

    resetTempFile(m_tempCarrierFile);
    resetTempFile(m_tempPayloadFile);

    QFileInfo tempPayloadFileInfo(m_tempPayloadFile->fileName());
    payloadImage->save(tempPayloadFileInfo.absoluteFilePath(), QStringLiteral("png").toStdString().c_str());

    QFileInfo tempCarrierFileInfo(m_tempCarrierFile->fileName());
    carrierImage->save(tempCarrierFileInfo.absoluteFilePath(), QStringLiteral("png").toStdString().c_str());

    QUrl tempPayloadFileUrl(m_tempPayloadFile->fileName());
    tempPayloadFileUrl.setScheme("file");
    m_payloadImageUrl = tempPayloadFileUrl;
    emit payloadImageUrlChanged();

    QUrl tempCarrierFileUrl(m_tempCarrierFile->fileName());
    tempCarrierFileUrl.setScheme("file");
    m_carrierImageUrl = tempCarrierFileUrl;
    emit carrierImageUrlChanged();

    qDebug() << "Carrier file = " << tempCarrierFileUrl;
    qDebug() << "Payload file = " << tempPayloadFileUrl;

    delete carrierImage;
    delete payloadImage;
    delete modulatedImage;

    return code::success;
}

void ImageProc::openImage(const int imageType) const
{
    switch(imageType) {
    case ImageProcUtil::ImageType::CarrierImage :
        QDesktopServices::openUrl(m_carrierImageUrl);
        break;
    case ImageProcUtil::ImageType::PayloadImage:
        QDesktopServices::openUrl(m_payloadImageUrl);
        break;
    case ImageProcUtil::ImageType::ModulatedImage:
        QDesktopServices::openUrl(m_resultImageUrl);
        break;
    default:
        qDebug() << "invalid input";
        return;
    }
}


bool ImageProc::saveImage(const QUrl &destinationUrl, const int imageType) const
{
    //perhaps a simpler way would be to just copy the image from tempDir

    QImage* image = new QImage();

    switch(imageType) {
    case ImageProcUtil::ImageType::CarrierImage :
        image->load(m_carrierImageUrl.url(QUrl::PreferLocalFile));
        break;
    case ImageProcUtil::ImageType::PayloadImage:
        image->load(m_payloadImageUrl.url(QUrl::PreferLocalFile));
        break;
    case ImageProcUtil::ImageType::ModulatedImage:
        image->load(m_resultImageUrl.url(QUrl::PreferLocalFile));
        break;
    default:
        qDebug() << "invalid input";
        return false;
    }

    if(image->isNull()) {
        qDebug() << "image contents were null after loading the image. Returning.";
        return false;
    }

    bool result = image->save(destinationUrl.url(QUrl::PreferLocalFile), QStringLiteral("png").toStdString().c_str());

    delete image;

    return result;
}
