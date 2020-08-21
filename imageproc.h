#ifndef IMAGEPROC_H
#define IMAGEPROC_H

#include <QObject>

class ImageProc : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString originalImageUrl
               MEMBER m_originalImageUrl
               RESET resetOriginalImageUrl
               NOTIFY originalImageUrlChanged)

public:
    explicit ImageProc(QObject *parent = nullptr);

    void resetOriginalImageUrl();

signals:
    void originalImageUrlChanged();

private:
    QString m_originalImageUrl;
};

#endif // IMAGEPROC_H
