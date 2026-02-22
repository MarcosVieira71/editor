#pragma once

#include <core/RGBA.h>

inline float to_lum(const RGBA& rgba)
{
    return 0.299 * rgba.r + rgba.g * 0.587 + rgba.b * 0.114;
}