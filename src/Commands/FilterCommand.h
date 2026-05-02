#pragma once
#include "Command.h"

#include <core/image/ImageData.h>

#include <functional>  

class FilterCommand : public Command {
    image::ImageData before;
    std::function<image::ImageData(const image::ImageData&)> filter;

public:
    explicit FilterCommand(std::function<image::ImageData(const image::ImageData&)> f)
        : filter(std::move(f)) {}

    void apply(image::ImageData& img) override {
        before = img.clone();
        img = filter(img);
    }

    void undo(image::ImageData& img) override {
        img = before;
    }
};
