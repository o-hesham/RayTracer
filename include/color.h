#pragma once

#include "interval.h"
#include "Math/vec3.h"

using color = vec3;

inline double linearToGamma(double linearComponent)
{
    if (linearComponent > 0)
        return std::sqrt(linearComponent);

    return 0;
}

void writeColor(std::ostream &out, const color &pixelColor)
{
    auto r = pixelColor.x();
    auto g = pixelColor.y();
    auto b = pixelColor.z();

    // Apply a linear to gamma transform for gamma 2
    r = linearToGamma(r);
    g = linearToGamma(g);
    b = linearToGamma(b);

    // Translate the [0,1] component values to the byte range [0,255]
    static const interval intensity(0.000, 0.999);
    int rbyte = static_cast<int>(256 * intensity.clamp(r));
    int gbyte = static_cast<int>(256 * intensity.clamp(g));
    int bbyte = static_cast<int>(256 * intensity.clamp(b));

    // Write out the pixel color components
    out << rbyte << " " << gbyte << " " << bbyte << std::endl;
}