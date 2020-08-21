#include "imageproc.h"
#include <QDebug>

namespace {
const QString placeholderImageUrl = "assets/images/placeholder";
}
ImageProc::ImageProc(QObject *parent) :
    QObject(parent),
    m_originalImageUrl(placeholderImageUrl),
    m_payloadImageUrl(placeholderImageUrl),
    m_resultImageUrl(placeholderImageUrl)

{

}

void ImageProc::resetOriginalImageUrl()
{
    m_originalImageUrl = placeholderImageUrl;
    emit originalImageUrlChanged();
}

void ImageProc::resetPayloadImageUrl()
{
    m_payloadImageUrl = placeholderImageUrl;
    emit payloadImageUrlChanged();
}

void ImageProc::resetResultImageUrl()
{
    m_resultImageUrl = placeholderImageUrl;
    emit resultImageUrlChanged();
}


