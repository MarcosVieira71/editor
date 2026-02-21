#include <core/Image.h>

Image::Image(std::string name, ImageData&& image_data)
        : _name(std::move(name)),
          _image_data(std::move(image_data))
    {}

const std::string& Image::name() const { return _name; }
const ImageData& Image::image_data() const { return _image_data; }
ImageData& Image::image_data() { return _image_data; }