#pragma once

#include "raytracer/scene/hittable.h"
#include "raytracer/material/material.h"

class camera
{
private:
    int imageHeight;          // Rendered image height
    double pixelSamplesScale; // Color scale factor for a sum of pixel samples (average)
    point3 center;            // Camera center
    point3 pixel00Loc;        // Location of pixel 0, 0
    vec3 pixelDeltaU;         // Offset to pixel to the right
    vec3 pixelDeltaV;         // Offset to pixel below
    vec3 u, v, w;             // Camera frame basis vectors
    vec3 defocusDiskU;        // Defocus disk horizontal radius
    vec3 defocusDiskV;        // Defocus disk vertical radius

    void initialize()
    {
        // Calculate the image height, and ensure that it's atleast 1
        imageHeight = static_cast<int>(imageWidth / aspectRatio);
        imageHeight = (imageHeight < 1) ? 1 : imageHeight;

        pixelSamplesScale = 1.0 / samplesPerPixel;

        center = lookFrom;

        // Determine viewport dimensions
        auto theta = degreesToRadians(vfov);
        auto h = std::tan(theta / 2);
        auto viewportHeight = 2 * h * focusDist;
        auto viewportWidth = viewportHeight * (static_cast<double>(imageWidth) / imageHeight);

        // Calculate the u,v,w unit basis vectors for the camera coordinate frame
        w = unitVector(lookFrom - lookAt); // backward
        u = unitVector(cross(vup, w));     // right
        v = cross(w, u);                   // up

        // Calculate the vectors across the horizontal and down the vertical viewport edges
        // Viewport is built in camera space
        auto viewportU = viewportWidth * u;   // Vector across viewport horziontal edge
        auto viewportV = viewportHeight * -v; // Vector down viewport vertoca; edge

        // Calculate the horizontal and vertical delta vectors from pixel to pixel
        pixelDeltaU = viewportU / imageWidth;
        pixelDeltaV = viewportV / imageHeight;

        // Calculate the location of the upper left pixel
        auto viewportUpperLeft = center - (focusDist * w) - viewportU / 2 - viewportV / 2;
        pixel00Loc = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV); // center of the top left pixel

        // Calculate the camera defocus disk basis vectors
        auto defocusRadius = focusDist * std::tan(degreesToRadians(defocusAngle / 2));
        defocusDiskU = u * defocusRadius;
        defocusDiskV = v * defocusRadius;
    }

    ray getRay(int i, int j) const
    {
        // Construct a camera ray originating from the defocus disk and directed at randomly sampled point around the pixel location i,j
        auto offset = sampleSquare();
        auto pixelSample = pixel00Loc + ((i + offset.x()) * pixelDeltaU) + ((j + offset.y()) * pixelDeltaV);

        auto rayOrigin = (defocusAngle <= 0) ? center : defocusDiskSample();
        auto rayDirection = pixelSample - rayOrigin;

        return ray(rayOrigin, rayDirection);
    }

    vec3 sampleSquare() const
    {
        // Return the vector to a random point in the [-0.5,-0.5]-[+0.5,+0.5] unit square
        return vec3(randomDouble() - 0.5, randomDouble() - 0.5, 0);
    }

    point3 defocusDiskSample() const
    {
        // Returns a random point in the camera defocus disk
        auto p = randomInUnitDisk();
        return center + (p[0] * defocusDiskU) + (p[1] * defocusDiskV);
    }

    color rayColor(const ray &r, int depth, const hittable &world) const
    {
        // If we've exceeded the ray bounce limit, no more light is gathered
        if (depth <= 0)
        {
            return color(0, 0, 0);
        }

        hitRecord rec;
        if (world.hit(r, interval(0.001, infinity), rec))
        {
            ray scattered;
            color attenuation; // How much the ray's color/intensity is reduced by the material

            if (rec.mat->scatter(r, rec, attenuation, scattered))
                return attenuation * rayColor(scattered, depth - 1, world);

            return color(0, 0, 0);
        }

        vec3 unitDirection = unitVector(r.direction());
        auto a = 0.5 * (unitDirection.y() + 1.0);                           // how much the ray is pointing upward
        return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0); // blend between white and blue (background color), a = 0 → all white, a = 1 → all blue, a = 0.5 → mix (lerp)
    }

public:
    double aspectRatio = 1.0; // Ratio of image width over height
    int imageWidth = 100;     // Rendered image width in pixel count
    int samplesPerPixel;      // Count of random samples for each pixel
    int maxDepth = 10;        // Maximum number of ray bounces  into scene

    double vfov = 90;                  // Vertical view angle
    point3 lookFrom = point3(0, 0, 0); // Point camera is looking from
    point3 lookAt = point3(0, 0, -1);  // Point camera is looking at
    vec3 vup = vec3(0, 1, 0);          // Camera-relative "up" direction

    double defocusAngle = 0; // Variation angle of rays through each pixel
    double focusDist = 10;   // Distance from camera lookfrom point to plane of perfect focus

    void render(const hittable &world)
    {
        initialize();

        std::cout << "P3\n"
                  << imageWidth << " " << imageHeight << "\n255" << std::endl;

        for (int j = 0; j < imageHeight; j++)
        {
            std::clog << "\rScanlines remaining: " << (imageHeight - j) << " " << std::flush;
            for (int i = 0; i < imageWidth; i++)
            {
                color pixelColor(0, 0, 0);
                for (int sample = 0; sample < samplesPerPixel; sample++)
                {
                    ray r = getRay(i, j);
                    pixelColor += rayColor(r, maxDepth, world);
                }

                writeColor(std::cout, pixelColor * pixelSamplesScale);
            }
        }

        std::clog << "\rDone.                      " << std::endl;
    }
};