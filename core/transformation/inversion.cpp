#include <core/image/ImageData.h>
#include <core/transformation/inversion.h>

namespace transformation
{

image::ImageData inversion(const image::ImageData& input)
{
    image::ImageData out = input.clone();

    auto* src = input.data();
    auto* dst = out.data();
    int total = input.width() * input.height();

    for (int i = 0; i < total; i++) {
        dst[i].r = 255 - src[i].r;
        dst[i].g = 255 - src[i].g;
        dst[i].b = 255 - src[i].b;
        dst[i].a = src[i].a;
    }

    return out;
}

}