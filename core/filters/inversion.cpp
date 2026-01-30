#include "inversion.h"

Image inversion(const Image& input)
{
    Image out = input.clone();

    auto* src = input.data();
    auto* dst = out.data();
    int total = input.width() * input.height();

    for (int i = 0; i < total; ++i) {
        dst[i].r = 255 - src[i].r;
        dst[i].g = 255 - src[i].r;
        dst[i].b = 255 - src[i].r;
        dst[i].a = src[i].a;
    }

    return out;
}