#ifndef UTIL_H
#define UTIL_H

#include <QMetaObject>
#include <QObject>
#include <QString>

namespace Util
{

Q_NAMESPACE           // required for meta object creation
enum ReturnCode
{
    UnknownError = 0,
    FileLoadError,
    InvalidParam,
    ImageLoadError,
    ImageProcessError,
    FileIOError,
    Success
};
Q_ENUM_NS(ReturnCode)

const quint8 minimumBitCount {1};
const quint8 maximumBitCount {3};
const QString imageFormat {QStringLiteral("png")};

}

#endif // UTIL_H
