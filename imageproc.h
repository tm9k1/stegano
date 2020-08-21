#ifndef IMAGEPROC_H
#define IMAGEPROC_H

#include <QObject>
#include <QUrl>

class ImageProc : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QUrl originalImageUrl
               MEMBER m_originalImageUrl
               NOTIFY originalImageUrlChanged)

    Q_PROPERTY(QUrl payloadImageUrl
               MEMBER m_payloadImageUrl
               NOTIFY payloadImageUrlChanged)

    Q_PROPERTY(QUrl resultImageUrl
               MEMBER m_resultImageUrl
               NOTIFY resultImageUrlChanged)

    Q_PROPERTY(quint8 bitCount
               MEMBER m_bitCount
               NOTIFY bitCountChanged)

public:
    explicit ImageProc(QObject *parent = nullptr);

signals:
    void originalImageUrlChanged();
    void payloadImageUrlChanged();
    void resultImageUrlChanged();
    void bitCountChanged();

private:
    QUrl m_originalImageUrl;
    QUrl m_payloadImageUrl;
    QUrl m_resultImageUrl;
    quint8 m_bitCount;
};

#endif // IMAGEPROC_H
