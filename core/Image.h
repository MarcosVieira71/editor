#pragma once

#include <string>
#include <vector>

struct RGBA{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
};

class QImage;

class Image
{
    public:
        Image(const char* path);
        Image(std::string path);

        Image(const Image&) = delete;
        Image& operator=(const Image&) = delete;

        Image(Image&&) noexcept = default;
        Image& operator=(Image&&) noexcept = default;
        
        ~Image();
        const RGBA& pixel(int x, int y) const;
        RGBA& pixel(int x, int y);

        const std::string& path() const;
        const int& width() const;
        const int& height() const;
        const int& channels() const;
        const unsigned char* data() const;
        Image clone() const;

    private:
        Image() = default;

        std::string _path;

        int _width;
        int _height;
        int _channels;
        std::vector<RGBA> _pixels;

};
