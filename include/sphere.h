#pragma once

#include "hittable.h"

class sphere : public hittable
{
private:
    point3 center;
    double radius;
    std::shared_ptr<material> mat;

public:
    sphere(const point3 &center, double radius, std::shared_ptr<material> mat)
        : center(center), radius(std::fmax(0, radius)), mat(mat) {}

    bool hit(const ray &r, interval rayT, hitRecord &rec) const override
    {
        vec3 oc = center - r.origin();
        auto a = r.direction().lengthSquared();
        auto h = dot(r.direction(), oc);
        auto c = oc.lengthSquared() - radius * radius;

        auto discriminant = h * h - a * c; //< 0 → no hit, = 0 → touches, > 0 → hits
        if (discriminant < 0)
            return false;

        auto sqrtd = std::sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range
        auto root = (h - sqrtd) / a; // Try the closest hit first
        // Is ray hit behind camera/too close or father than an object already found
        if (!rayT.surrounds(root))
        {
            root = (h + sqrtd) / a; // Try second hit (exit point)
            if (!rayT.surrounds(root))
                return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        vec3 outwardNormal = (rec.p - center) / radius;
        rec.setFaceNormal(r, outwardNormal);
        rec.mat = mat;

        return true;
    }
};