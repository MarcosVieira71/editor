#pragma once
#include "Command.h"

#include <core/Image.h>

#include <functional>  

class FilterCommand : public Command {
    Image before;
    std::function<Image(const Image&)> filter;

public:
    explicit FilterCommand(std::function<Image(const Image&)> f)
        : filter(std::move(f)) {}

    void apply(Image& img) override {
        before = img.clone();
        img = filter(img);
    }

    void undo(Image& img) override {
        img = before;
    }
};
