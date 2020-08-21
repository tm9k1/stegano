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

    Q_PROPERTY(QString payloadImageUrl
               MEMBER m_payloadImageUrl
               RESET resetPayloadImageUrl
               NOTIFY payloadImageUrlChanged)

    Q_PROPERTY(QString resultImageUrl
               MEMBER m_resultImageUrl
               RESET resetResultImageUrl
               NOTIFY resultImageUrlChanged)

public:
    explicit ImageProc(QObject *parent = nullptr);

    void resetOriginalImageUrl();
    void resetPayloadImageUrl();
    void resetResultImageUrl();

signals:
    void originalImageUrlChanged();
    void payloadImageUrlChanged();
    void resultImageUrlChanged();

private:
    QString m_originalImageUrl;
    QString m_payloadImageUrl;
    QString m_resultImageUrl;
};

#endif // IMAGEPROC_H
