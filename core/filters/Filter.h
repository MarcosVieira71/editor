#pragma once

#include "Image.h"

class Filter {
public:
    virtual ~Filter() = default;

    virtual Image apply(const Image& input) const = 0;
};