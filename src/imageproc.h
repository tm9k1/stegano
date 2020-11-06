#ifndef IMAGEPROC_H
#define IMAGEPROC_H

#include <QUrl>
#include <QTemporaryFile>
#include <QPointer>
#include <QUrl>

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
    static ImageProc* getInstance();
    ~ImageProc();
    Q_INVOKABLE int hideImage();
    Q_INVOKABLE int retrieveImage();

    Q_INVOKABLE bool openImage(const QUrl &url) const;
    Q_INVOKABLE bool saveImage(const QUrl &sourceUrl, const QUrl &destinationUrl) const;

    void resetTempFile(QPointer<QTemporaryFile> &tempFile);

signals:
    void carrierImageUrlChanged();
    void payloadImageUrlChanged();
    void modulatedImageUrlChanged();
    void bitCountChanged();

private:
    explicit ImageProc(QObject *parent = nullptr);
    static ImageProc* m_instance;
    quint8 m_bitCount;
    QUrl m_carrierImageUrl; // up to date with the GUI
    QUrl m_payloadImageUrl; // ,,
    QUrl m_modulatedImageUrl;  // ,, int m_bitCount;

    // these should stay alive till the end of program/ end of this object
    QPointer<QTemporaryFile> m_tempCarrierImageFile;
    QPointer<QTemporaryFile> m_tempPayloadImageFile;
    QPointer<QTemporaryFile> m_tempModulatedImageFile;
};

#endif // IMAGEPROC_H
