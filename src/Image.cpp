#include "Image.h"

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
}


Image::~Image()
{
    stbi_image_free(_data);
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