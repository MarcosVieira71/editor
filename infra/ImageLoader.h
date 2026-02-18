#pragma once

#include <string>

#include <core/Image.h>

class ImageLoader {
public:
    static Image load(const std::string& path);

    static void save(const Image& image, const std::string& path, const std::string& extension);
};
