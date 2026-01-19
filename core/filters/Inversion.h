#pragma once

#include "Filter.h"

class Inversion : public Filter {
public:
    ~Inversion() = default;

    virtual Image apply(const Image& input) const override final;
};