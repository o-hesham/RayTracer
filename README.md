# RayTracer

A small C++17 ray tracer that renders a scene to a PPM image.

## Features
- Diffuse, metal, and dielectric (glass) materials
- Anti-aliased rendering via multiple samples per pixel
- Gamma-corrected color output
- Depth of field with a defocus disk camera model
- Randomized scene generation with many spheres

## Requirements
- CMake 3.10+
- C++17 compiler (MSVC, clang, or GCC)

## Build (Windows, Visual Studio generator)
```
cmake -S . -B build
cmake --build build --config Release
```

## Run
```
build/Release/RayTracer.exe > image.ppm
```

The renderer writes the PPM image to stdout and progress to stderr.

## Project Layout
- include/raytracer/core: core utilities, constants, and intervals
- include/raytracer/math: vector math
- include/raytracer/material: materials and scattering
- include/raytracer/render: camera, ray, and color
- include/raytracer/scene: geometry and hittables
- src: application entry point

## Configuration Options
All configuration lives in src/main.cpp. Common knobs:
- imageWidth: output width in pixels (height is derived from aspectRatio)
- aspectRatio: width/height ratio
- samplesPerPixel: anti-aliasing quality (higher is slower)
- maxDepth: maximum ray bounce depth
- vfov: vertical field of view in degrees
- lookFrom, lookAt, vup: camera position, target, and up vector
- defocusAngle: aperture angle for depth of field (0 disables)
- focusDist: distance to the plane of perfect focus

## Customize
Edit src/main.cpp to change resolution, camera, material mix, and scene setup.
