#pragma once

#include "raytracer/scene/hittable.h"

class material
{
public:
    virtual ~material() = default;

    virtual bool scatter(const ray &rIn, const hitRecord &rec, color &attenuation, ray &scattered) const
    {
        return false;
    }
};

class lambertian : public material // matte
{
private:
    color albedo; // How much light a surface reflects

public:
    lambertian(const color &albedo)
        : albedo(albedo) {}

    bool scatter(const ray &rIn, const hitRecord &rec, color &attenuation, ray &scattered) const override
    {
        // Pick a random direction, but bias it toward the surface normal
        auto scatterDirection = rec.normal + randomUnitVector();
        // vec3 direction = randomOnHemisphere(rec.normal);(unifrom: try later)

        // Catch degenrate scatter direction
        if (scatterDirection.nearZero())
            scatterDirection = rec.normal;

        scattered = ray(rec.p, scatterDirection);
        attenuation = albedo; // Keeps a fraction of its light equal to the material’s color reflectivity when bouncing off

        return true;
    }
};

class metal : public material
{
private:
    color albedo; // How much light a surface reflects
    double fuzz;  // Blur (so metal doesnt behave like a mirror)

public:
    metal(const color &albedo, double fuzz)
        : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

    bool scatter(const ray &rIn, const hitRecord &rec, color &attenuation, ray &scattered) const override
    {
        vec3 reflected = reflect(rIn.direction(), rec.normal);
        reflected = unitVector(reflected) + (fuzz * randomUnitVector());
        scattered = ray(rec.p, reflected);
        attenuation = albedo; // Keeps a fraction of its light equal to the material’s color reflectivity when bouncing off

        return (dot(scattered.direction(), rec.normal) > 0); // Make sure the reflected ray actually goes out of the surface bec of fuzz
    }
};

class dielectric : public material
{
private:
    // Refractive index in vacuum or air, or the ratio of the material's refractive index over the refractive index of the enclosing media
    double refractionIndex;

    static double reflectance(double cosine, double refractionIndex)
    {
        // Use Schlick's approximation for reflectance
        // Reflectivity at normal incidence
        auto r0 = (1 - refractionIndex) / (1 + refractionIndex);
        r0 = r0 * r0;

        return r0 + (1 - r0) * std::pow((1 - cosine), 5); // Reflection grow as angle becomes more grazing
    }

public:
    dielectric(double refractionIndex)
        : refractionIndex(refractionIndex) {}

    bool scatter(const ray &rIn, const hitRecord &rec, color &attenuation, ray &scattered) const override
    {
        attenuation = color(1.0, 1.0, 1.0);
        double ri = rec.frontFace ? (1.0 / refractionIndex) : refractionIndex;

        vec3 unitDirection = unitVector(rIn.direction());
        double cosTheta = std::fmin(dot(-unitDirection, rec.normal), 1.0);
        double sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);

        bool cannotRefract = ri * sinTheta > 1.0;
        vec3 direction;

        if (cannotRefract || reflectance(cosTheta, ri) > randomDouble())
            direction = reflect(unitDirection, rec.normal);
        else
            direction = refract(unitDirection, rec.normal, ri);

        scattered = ray(rec.p, direction);

        return true;
    }
};
