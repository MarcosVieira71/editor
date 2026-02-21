#include "grayscale.h"

ImageData grayscale(const ImageData& input)
{
    ImageData out = input.clone();

    const auto* src = input.data();
    auto* dst = out.data();
    int total = input.width() * input.height();

    for (int i = 0; i < total; i++) {
        unsigned char gray =
            0.22f * src[i].r +
            0.72f * src[i].g +
            0.07f * src[i].b;

        dst[i].r = gray;
        dst[i].g = gray;
        dst[i].b = gray;
        dst[i].a = src[i].a;
    }

    return out;
}