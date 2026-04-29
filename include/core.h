#pragma once

#include <cmath>
#include <cstdlib>
#include <random>
#include <iostream>
#include <limits>
#include <memory>

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions
inline double degreesToRadians(double degrees)
{
    return degrees * pi / 180.0;
}

inline double randomDouble()
{
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double randomDouble(double min, double max)
{
    // Returns a random real in [min,max)
    return min + (max - min) * randomDouble();
}

// Common Headers
#include "color.h"
#include "ray.h"
#include "Math/vec3.h"
#include "interval.h"