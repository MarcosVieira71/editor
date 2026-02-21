#pragma once
#include "Command.h"

#include <core/ImageData.h>

#include <functional>  

class FilterCommand : public Command {
    ImageData before;
    std::function<ImageData(const ImageData&)> filter;

public:
    explicit FilterCommand(std::function<ImageData(const ImageData&)> f)
        : filter(std::move(f)) {}

    void apply(ImageData& img) override {
        before = img.clone();
        img = filter(img);
    }

    void undo(ImageData& img) override {
        img = before;
    }
};
