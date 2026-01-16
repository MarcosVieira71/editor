#include "Image.h"

#include <QImage>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <stdexcept>
#include <string>

Image::Image(const char* path) : _path(path)
{
    //load images with 4 channels for qt integration, we can change this later
    _data = stbi_load(path, &_width, &_height, &_channels, 4);
    if(!_data){
        throw std::runtime_error("Error loading data");
    }
    _channels = 4;
}

Image::Image(std::string path) : Image(path.c_str())
{
}

Image::Image(Image&& other) noexcept
    : _path(std::move(other._path)),
      _width(other._width),
      _height(other._height),
      _channels(other._channels),
      _data(other._data)
{
    other._data = nullptr;
}

Image& Image::operator=(Image&& other) noexcept
{
    if (this != &other)
    {
        stbi_image_free(_data);

        _path = std::move(other._path);
        _width = other._width;
        _height = other._height;
        _channels = other._channels;
        _data = other._data;

        other._data = nullptr;
    }
    return *this;
}



Image::~Image()
{
    stbi_image_free(_data);
}

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
    return _data;
}


