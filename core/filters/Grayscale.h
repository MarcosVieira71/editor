#pragma once

#include "Filter.h"

class Grayscale : public Filter {
public:
    ~Grayscale() = default;

    virtual Image apply(const Image& input) const override final;
};