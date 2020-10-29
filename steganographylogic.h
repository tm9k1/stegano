#ifndef STEGANOGRAPHYLOGIC_H
#define STEGANOGRAPHYLOGIC_H

#include <QImage>

class SteganographyLogic
{
public:
    SteganographyLogic();
    // create your implementation to apply a different logic for steganography
    static bool hideImage(const QImage &carrierImage, const QImage &payloadImage, QImage &modulatedImage, const int m_bitCount = -1);
    static bool retrieveImages(QImage &carrierImage, QImage &hiddenImage, const QImage &modulatedImage, const int m_bitCount = -1);

};

#endif // STEGANOGRAPHYLOGIC_H
