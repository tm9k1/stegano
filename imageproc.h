#ifndef IMAGEPROC_H
#define IMAGEPROC_H

#include <QObject>
#include <QUrl>
#include <QTemporaryFile>
#include <QPointer>
#include <QUrl>

namespace ImageProcUtil
{
Q_NAMESPACE           // required for meta object creation
enum ImageType
{
    CarrierImage=0,
    PayloadImage,
    ModulatedImage
};

enum ReturnCode{
    Success=0,
    FileLoadError,
    InvalidBitCount,
    ImageLoadError,
    ImageProcessError,
    UnknownError
};

const quint8 minimumBitCount = 1;
const quint8 maximumBitCount = 3;
const QString imageFormat = QStringLiteral("png");

Q_ENUM_NS(ImageType)  // register the enum in meta object data
Q_ENUM_NS(ReturnCode)
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

    Q_PROPERTY(QUrl modulatedImageUrl
               MEMBER m_modulatedImageUrl
               NOTIFY modulatedImageUrlChanged)

    Q_PROPERTY(quint8 bitCount
               MEMBER m_bitCount
               NOTIFY bitCountChanged)

public:

    explicit ImageProc(QObject *parent = nullptr);
    ~ImageProc();
    Q_INVOKABLE int hideImage();
    Q_INVOKABLE int retrieveImage();

    Q_INVOKABLE bool openImage(const int image) const;
    Q_INVOKABLE bool saveImage(const QUrl &destinationUrl, const int image) const;

    void resetTempFile(QPointer<QTemporaryFile> &tempFile);

signals:
    void carrierImageUrlChanged();
    void payloadImageUrlChanged();
    void modulatedImageUrlChanged();
    void bitCountChanged();

private:
    QUrl m_carrierImageUrl; // up to date with the GUI
    QUrl m_payloadImageUrl; // ,,
    QUrl m_modulatedImageUrl;  // ,,
    int m_bitCount;

    // these should stay alive till the end of program/ end of this object
    QPointer<QTemporaryFile> m_tempCarrierImageFile;
    QPointer<QTemporaryFile> m_tempPayloadImageFile;
    QPointer<QTemporaryFile> m_tempModulatedImageFile;

};

#endif // IMAGEPROC_H
