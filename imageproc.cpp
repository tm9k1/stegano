#include "imageproc.h"
#include <QDebug>

ImageProc::ImageProc(QObject *parent) :
    QObject(parent),
    m_originalImageUrl("assets/images/placeholder"),
    m_payloadImageUrl("assets/images/placeholder"),
    m_resultImageUrl("assets/images/placeholder")

{

}

void ImageProc::resetOriginalImageUrl()
{
    m_originalImageUrl = "assets/images/placeholder";
    emit originalImageUrlChanged();
}

void ImageProc::resetPayloadImageUrl()
{
    m_payloadImageUrl = "assets/images/placeholder";
    emit payloadImageUrlChanged();
}

void ImageProc::resetResultImageUrl()
{
    m_resultImageUrl = "assets/images/placeholder";
    emit resultImageUrlChanged();
}


