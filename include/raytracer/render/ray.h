#pragma once

#include "raytracer/math/vec3.h"

class ray
{
private:
    point3 orig;
    vec3 dir;

public:
    ray() {}

    ray(const point3 &origin, const vec3 &direction)
        : orig(origin), dir(direction) {}

    const point3 &origin() const { return orig; }
    const vec3 &direction() const { return dir; }

    // Returns the position on the ray at distance t
    point3 at(double t) const
    {
        return orig + t * dir;
    }
};