#include "ImageData.h"



#include <cstring>
#include <stdexcept>
#include <string>

ImageData::ImageData(int width, int height, int channels, const std::string& path)
    : _width(width), _height(height), _channels(channels), _path(path)
{
    _pixels.resize(width * height);
}

ImageData ImageData::clone() const
{
    ImageData copy = ImageData();
    copy._width = _width;
    copy._height = _height;
    copy._channels = _channels;
    copy._pixels = _pixels;
    copy._path = _path;
    return copy;
}

const RGBA& ImageData::pixel(int x, int y) const
{
    if (x < 0 || x >= _width || y < 0 || y >= _height) {
        throw std::out_of_range("Pixel out of bounds");
    }
    return _pixels[y * _width + x];
}

RGBA& ImageData::pixel(int x, int y)
{
    if (x < 0 || x >= _width || y < 0 || y >= _height) {
        throw std::out_of_range("Pixel out of bounds");
    }
    return _pixels[y * _width + x];
}

ImageData::~ImageData() = default;

const std::string& ImageData::path() const
{
    return _path;
}

const int& ImageData::width() const
{
    return _width;
}

const int& ImageData::height() const
{
    return _height;
}

const int& ImageData::channels() const
{
    return _channels;
}

const RGBA* ImageData::data() const
{
    return _pixels.data();
}

RGBA* ImageData::data()
{
    return _pixels.data();
}

const unsigned char* ImageData::bytes() const
{
    return reinterpret_cast<const unsigned char*>(_pixels.data());
}

const std::vector<RGBA>& ImageData::buffer() const
{
    return _pixels;    
}