#include "core/transformation/binarization.h"

#include "core/image/ImageData.h"
#include "core/utils/to_lum.h"

#include <cmath>
#include <vector>

namespace transformation
{

int otsu_method(const std::vector<image::RGBA>& buffer)
{
    std::vector<int> hist(256, 0);

    for(const auto& rgba : buffer)
    {
        auto lum = utils::to_lum(rgba);
        hist[static_cast<int>(std::round(lum))]++;
    }

    int total = 0;
    for (auto histValue : hist)
        total += histValue;

    double sum = 0.0;
    for(int i = 0; i < 256; i++)
        sum += i * hist[i]; 
    
    double muT = sum/total;
    int bestThreshold = 0;
    double maxVar = 0.0;

    int w0_count = 0;      
    double sum0 = 0.0;     

    for (int t = 0; t < 256; t++)
    {
        w0_count += hist[t];
        sum0 += t * hist[t];

        if (w0_count == 0)
            continue;

        int w1_count = total - w0_count;
        if (w1_count == 0)
            break;

        double numerator = (muT * w0_count - sum0);
        numerator = numerator * numerator;

        double denominator = static_cast<double>(w0_count) * static_cast<double>(w1_count);

        double varBetween = numerator / denominator;

        if (varBetween > maxVar)
        {
            maxVar = varBetween;
            bestThreshold = t;
        }
    }

    return bestThreshold;
}


image::ImageData binarization(const image::ImageData& input)
{
    auto threshold = otsu_method(input.buffer());

    image::ImageData out = input.clone();

    const auto* src = input.data();
    auto* dst = out.data();
    int total = input.width() * input.height();
    for (int i = 0; i < total; i++) {
        auto lum = 0.299 * src[i].r + src[i].g * 0.587 + src[i].b * 0.114;
        unsigned char value = lum > threshold ? 255 : 0;
        dst[i] = {value, value, value, src[i].a}; 
    }

    return out;
}

}
