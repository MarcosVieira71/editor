#pragma once

#include <core/image/RGBA.h>

namespace utils
{

inline float to_lum(const image::RGBA& rgba)
{
    return 0.299 * rgba.r + rgba.g * 0.587 + rgba.b * 0.114;
}

}
