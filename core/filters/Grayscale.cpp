#include "Grayscale.h"

Image Grayscale::apply(const Image& input) const
{
    Image out = input.clone();

    for (int y = 0; y < input.height(); ++y) {
        for (int x = 0; x < input.width(); ++x) {

            const auto& src = input.pixel(x, y);
            auto& dst = out.pixel(x, y);

            unsigned char gray = static_cast<unsigned char>(
                0.22 * src.r +
                0.72 * src.g +
                0.07 * src.b
            );

            dst.r = gray;
            dst.g = gray;
            dst.b = gray;
            dst.a = src.a;
        }
    }

    return out;
}
