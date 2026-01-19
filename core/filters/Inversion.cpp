#include "Inversion.h"


Image Inversion::apply(const Image& input) const
{
    Image out = input.clone();

    for (int y = 0; y < input.height(); ++y) {
        for (int x = 0; x < input.width(); ++x) {

            const auto& src = input.pixel(x, y);
            auto& dst = out.pixel(x, y);
            
            dst.r = 255 - src.r;
            dst.g = 255 - src.g;
            dst.b = 255 - src.b;
            dst.a = src.a;
        }
    }

    return out;
}
