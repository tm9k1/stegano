#ifndef IMAGEPROC_H
#define IMAGEPROC_H

#include <QObject>

class ImageProc : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString originalImageUrl
               READ getOriginalImageUrl
               WRITE setOriginalImageUrl
               RESET resetOriginalImageUrl
               NOTIFY originalImageUrlChanged)

public:
    explicit ImageProc(QObject *parent = nullptr);

    QString getOriginalImageUrl() const;
    void setOriginalImageUrl(const QString &newOriginalImageUrl);
    void resetOriginalImageUrl();

signals:
    void originalImageUrlChanged();

private:
    QString m_originalImageUrl;

};

#endif // IMAGEPROC_H
