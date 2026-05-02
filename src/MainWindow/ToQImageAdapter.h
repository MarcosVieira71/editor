#pragma once

#include <core/image/ImageData.h>

#include <QImage>

inline QImage toQImage(const image::ImageData& image)
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