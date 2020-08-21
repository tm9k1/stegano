#include "imageproc.h"
#include <QDebug>

ImageProc::ImageProc(QObject *parent) : QObject(parent)
{
    m_originalImageUrl = "assets/images/placeholder";
}

QString ImageProc::getOriginalImageUrl() const
{
    return m_originalImageUrl;
}

void ImageProc::setOriginalImageUrl(const QString &newOriginalImageUrl)
{
    if (m_originalImageUrl == newOriginalImageUrl) {
        return;
    } else if (newOriginalImageUrl == "") {
        resetOriginalImageUrl();
    } else {
        m_originalImageUrl = newOriginalImageUrl;
    }
    emit originalImageUrlChanged();
}

void ImageProc::resetOriginalImageUrl()
{
    m_originalImageUrl = "assets/images/placeholder";
    emit originalImageUrlChanged();
}
