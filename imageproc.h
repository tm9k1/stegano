#ifndef IMAGEPROC_H
#define IMAGEPROC_H

#include <QObject>
#include <QUrl>
#include <QTemporaryFile>
#include <QPointer>

namespace ImageProcUtil
{
Q_NAMESPACE           // required for meta object creation
enum ImageType
{
    CarrierImage=0,
    PayloadImage,
    ModulatedImage
};
Q_ENUM_NS(ImageType)  // register the enum in meta object data
}

class ImageProc : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QUrl carrierImageUrl
               MEMBER m_carrierImageUrl
               NOTIFY carrierImageUrlChanged)

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
    ~ImageProc();
    Q_INVOKABLE int hideImage();
    Q_INVOKABLE int retrieveImage();

    Q_INVOKABLE void openImage(int image) const;
    Q_INVOKABLE void saveImage(int image) const;

    void resetTempFile(QPointer<QTemporaryFile> &tempFile);

signals:
    void carrierImageUrlChanged();
    void payloadImageUrlChanged();
    void resultImageUrlChanged();
    void bitCountChanged();

private:
    QUrl m_carrierImageUrl;
    QUrl m_payloadImageUrl;
    QUrl m_resultImageUrl;
    int m_bitCount;

    QPointer<QTemporaryFile> m_tempCarrierFile;
    QPointer<QTemporaryFile> m_tempPayloadFile;

    QPointer<QTemporaryFile> m_tempResultFile;

};

#endif // IMAGEPROC_H
