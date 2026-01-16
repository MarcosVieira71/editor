#pragma once

#include <string>
class QImage;

class Image
{
    public:

        Image() = delete;
        Image(const char* path);
        Image(std::string path);

        Image(const Image&) = delete;
        Image& operator=(const Image&) = delete;

        Image(Image&& other) noexcept;
        Image& operator=(Image&& other) noexcept;

        ~Image();

        const std::string& path() const;
        const int& width() const;
        const int& height() const;
        const int& channels() const;
        const unsigned char* data() const;

    private:
        std::string _path;

        int _width;
        int _height;
        int _channels;
        unsigned char* _data;

};
