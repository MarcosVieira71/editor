#include "Image.h"

#include <QImage>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <cstring>
#include <stdexcept>
#include <string>

Image::Image(const char* path) : _path(path)
{
    //load images with 4 channels for qt integration, we can change this later
    auto data = stbi_load(path, &_width, &_height, &_channels, 4);
    if(!data){
        throw std::runtime_error("Error loading data");
    }
    _channels = 4;
    _pixels.resize(_width * _height);
    std::memcpy(_pixels.data(), data, _pixels.size() * sizeof(RGBA));

    stbi_image_free(data);

}

Image::Image(std::string path) : Image(path.c_str())
{
}

Image Image::clone() const
{
    Image copy = Image();
    copy._width = _width;
    copy._height = _height;
    copy._channels = _channels;
    copy._pixels = _pixels;
    copy._path = _path;
    return copy;
}

const RGBA& Image::pixel(int x, int y) const
{
    if (x < 0 || x >= _width || y < 0 || y >= _height) {
        throw std::out_of_range("Pixel out of bounds");
    }
    return _pixels[y * _width + x];
}

RGBA& Image::pixel(int x, int y)
{
    if (x < 0 || x >= _width || y < 0 || y >= _height) {
        throw std::out_of_range("Pixel out of bounds");
    }
    return _pixels[y * _width + x];
}

Image::~Image() = default;

const std::string& Image::path() const
{
    return _path;
}

const int& Image::width() const
{
    return _width;
}

const int& Image::height() const
{
    return _height;
}

const int& Image::channels() const
{
    return _channels;
}

const unsigned char* Image::data() const
{
    return reinterpret_cast<const unsigned char*>(_pixels.data());
}


