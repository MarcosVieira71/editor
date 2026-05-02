#pragma once

#include <string>

#include <core/ImageData.h>

class ImageLoader {
public:
    static ImageData load(const std::string& path);

    static void save(const ImageData& image, const std::string& path, const std::string& extension);
};
