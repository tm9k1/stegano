#include "imageproc.h"
#include <QDebug>
#include <QImage>
#include <QFile>

namespace {
    enum code{
        success=0,
        fileLoadError,
    };
}

ImageProc::ImageProc(QObject *parent) : QObject(parent)
{

}

int ImageProc::hideImage()
{
    if(!m_originalImageUrl.isLocalFile() || !m_payloadImageUrl.isLocalFile() || !m_resultImageUrl.isLocalFile()) {
        return code::fileLoadError;
    }
    qDebug() << m_originalImageUrl;
    qDebug() << m_payloadImageUrl;
    qDebug() << m_resultImageUrl;
    qDebug() << m_bitCount;
    QImage someImage = QImage(m_originalImageUrl.url(QUrl::PreferLocalFile));

    if(someImage.isNull()) return 1;
    qDebug() << "hellooo";
    return code::success;
}
