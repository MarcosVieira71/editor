#pragma once

#include <core/ImageData.h>

#include <string>

class Image
{
public:
    Image(std::string name, ImageData&& image_data);

    const std::string& name() const;
    const ImageData& image_data() const;
    ImageData& image_data();

private:
    std::string _name;
    ImageData _image_data;
};