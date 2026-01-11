#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <stdexcept>
#include <string>


class Image
{
    public:

        Image() = delete;
        Image(const char* path) : _path(path)
        {
            _data = stbi_load(path, &_width, &_height, &_channels, 0);
            if(!_data){
                throw std::runtime_error("Error loading data");
            }
        }

        Image(const Image&) = delete;
        Image& operator=(const Image&) = delete;

        ~Image()
        {
            stbi_image_free(_data);
        }

    private:
        const std::string _path;

        int _width;
        int _height;
        int _channels;
        unsigned char* _data;

};
