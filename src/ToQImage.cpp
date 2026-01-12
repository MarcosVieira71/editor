#include "ToQImage.h"
#include "Image.h"

QImage to_QImage(const Image& image)
{
    QImage qimg(
        image.data(),
        image.width(),
        image.height(),
        image.width() * 4,
        QImage::Format_RGBA8888
    );

    return qimg.copy();
}
