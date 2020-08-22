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

bool ImageProc::hideImage()
{
    qDebug() << m_originalImageUrl;
    qDebug() << m_payloadImageUrl;
    qDebug() << m_resultImageUrl;
    qDebug() << m_bitCount;
    QImage someImage = QImage(m_originalImageUrl.url(QUrl::PreferLocalFile));

    if(someImage.isNull()) return 1;
    qDebug() << "hellooo";
    return code::success;
}
