#include "Image.h"



#include <cstring>
#include <stdexcept>
#include <string>

Image::Image(int width, int height, int channels, const std::string& path)
    : _width(width), _height(height), _channels(channels), _path(path)
{
    _pixels.resize(width * height);
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

const RGBA* Image::data() const
{
    return _pixels.data();
}

RGBA* Image::data()
{
    return _pixels.data();
}

const unsigned char* Image::bytes() const
{
    return reinterpret_cast<const unsigned char*>(_pixels.data());
}

const std::vector<RGBA>& Image::buffer() const
{
    return _pixels;    
}