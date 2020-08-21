#include "imageproc.h"
#include <QDebug>

ImageProc::ImageProc(QObject *parent) : QObject(parent),
                                        m_originalImageUrl("assets/images/placeholder")
{

}

void ImageProc::resetOriginalImageUrl()
{
    m_originalImageUrl = "assets/images/placeholder";
    emit originalImageUrlChanged();
}
