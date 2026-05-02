#include <infra/images/ImageLoader.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <infra/images/ImageLoader.h>
#include <core/image/ImageData.h>

#include <stdexcept>
#include <cstring>

image::ImageData ImageLoader::load(const std::string& path)
{
    int width, height, channels;

    unsigned char* data = stbi_load(
        path.c_str(),
        &width,
        &height,
        &channels,
        4
    );

    if (!data)
        throw std::runtime_error("Failed to load image");

    image::ImageData img(width, height, 4, path);

    std::memcpy(
        img.data(),
        data,
        width * height * sizeof(image::RGBA)
    );

    stbi_image_free(data);

    return img;
}


void ImageLoader::save(const image::ImageData& image, const std::string& path, const std::string& extension)
{
    const int width = image.width();
    const int height = image.height();
    const int channels = image.channels();

    const unsigned char* data = image.bytes(); 

    int result = 0;
    if (extension == "png")
    {
        result = stbi_write_png(path.c_str(),
                                width,
                                height,
                                channels,
                                data,
                                width * channels);
    }
    else if (extension == "jpg" || extension == "jpeg")
    {
        const int quality = 100;
        result = stbi_write_jpg(path.c_str(),
                                width,
                                height,
                                channels,
                                data,
                                quality);
    }
    else if (extension == "bmp")
    {
        result = stbi_write_bmp(path.c_str(),
                                width,
                                height,
                                channels,
                                data);
    }
    else
    {
        throw std::runtime_error("Unsupported image format: " + extension);
    }

    if (result == 0)
        throw std::runtime_error("Failed to write image to disk");
}
