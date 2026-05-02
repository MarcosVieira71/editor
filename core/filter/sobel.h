#pragma once

namespace image
{
    class ImageData;
}

namespace filter
{
    image::ImageData sobel(const image::ImageData& input);
}
