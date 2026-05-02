#pragma once

#include <string>

namespace image
{
    class ImageData;
}

class ImageLoader {
public:
    static image::ImageData load(const std::string& path);

    static void save(const image::ImageData& image, const std::string& path, const std::string& extension);
};
