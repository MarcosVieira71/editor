#include "core/filters/sobel.h"
#include "core/utils/to_lum.h"

#include <cmath>

ImageData sobel(const ImageData& input)
{
    ImageData out = input.clone();

    int w = input.width();
    int h = input.height();

    auto* src = input.data();
    auto* dst = out.data();

    int sX[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    int sY[3][3] = {
        {-1, -2, -1},
        { 0,  0,  0},
        { 1,  2,  1}
    };

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {

            int sumX = 0;
            int sumY = 0;

            for (int ky = -1; ky <= 1; ky++) {
                for (int kx = -1; kx <= 1; kx++) {
                    int nx = x + kx;
                    int ny = y + ky;

                    //padding if out of borders
                    if (nx < 0) nx = 0;
                    if (nx >= w) nx = w - 1;
                    if (ny < 0) ny = 0;
                    if (ny >= h) ny = h - 1;

                    const RGBA& p = input.pixel(nx, ny);

                    int gray = static_cast<int>(to_lum(p));

                    sumX += gray * sX[ky + 1][kx + 1];
                    sumY += gray * sY[ky + 1][kx + 1];
                }
            }

            int magnitude = static_cast<int>(std::sqrt(sumX * sumX + sumY * sumY));
            if (magnitude > 255) magnitude = 255;
            if (magnitude < 0) magnitude = 0;

            dst[y * w + x].r = magnitude;
            dst[y * w + x].g = magnitude;
            dst[y * w + x].b = magnitude;
            dst[y * w + x].a = src[y * w + x].a; 
        }
    }

    return out;
}