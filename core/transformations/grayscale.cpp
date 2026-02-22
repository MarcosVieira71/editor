#include "core/transformations/grayscale.h"

#include "core/utils/to_lum.h"

ImageData grayscale(const ImageData& input)
{
    ImageData out = input.clone();

    const auto* src = input.data();
    auto* dst = out.data();
    int total = input.width() * input.height();

    for (int i = 0; i < total; i++) {
        unsigned char gray = static_cast<unsigned char>(to_lum(src[i]));

        dst[i].r = gray;
        dst[i].g = gray;
        dst[i].b = gray;
        dst[i].a = src[i].a;
    }

    return out;
}