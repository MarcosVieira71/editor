#pragma once

#include <Image.h>

#include <QImage>

inline QImage toQImage(const Image& image)
{
    QImage qimg(
        image.bytes(),
        image.width(),
        image.height(),
        image.width() * image.channels(),
        QImage::Format_RGBA8888
    );

    return qimg.copy();
}