#pragma once

#include <string>
#include <vector>

struct RGBA{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
};

class Image
{
    public:
        Image() = default;
        Image(int width, int height, int channels, const std::string& path);

        ~Image();
        const RGBA& pixel(int x, int y) const;
        RGBA& pixel(int x, int y);

        const std::string& path() const;
        const int& width() const;
        const int& height() const;
        const int& channels() const;
        const unsigned char* bytes() const;
        const RGBA* data() const;
        const std::vector<RGBA>& buffer() const;
        RGBA* data();

        Image clone() const;
        

    private:
  
        std::string _path;

        int _width;
        int _height;
        int _channels;
        std::vector<RGBA> _pixels;

};
