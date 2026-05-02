#include <core/image/Image.h>

namespace image
{

  Image::Image(std::string name, ImageData&& image_data, std::size_t id)
        : _name(std::move(name)),
          _image_data(std::move(image_data)),
          _id(id)
    {}

const std::string& Image::name() const { return _name; }
const ImageData& Image::image_data() const { return _image_data; }
ImageData& Image::image_data() { return _image_data; }
const std::size_t Image::id() const {return _id; }  

}
