#pragma once

#include "core.h"

class material;

struct hitRecord
{
    point3 p;                      // Hit point
    vec3 normal;                   // surface normal at hit
    std::shared_ptr<material> mat; // material at hit point
    double t;                      // ray parameter (distance along ray)
    bool frontFace;                // whether ray hit outside or inside

    void setFaceNormal(const ray &r, const vec3 &outwardNormal)
    {
        // Sets the hit record normal vector
        // Note: the parameter "outwardNormal" is assumed to have unit length
        frontFace = dot(r.direction(), outwardNormal) < 0;
        normal = frontFace ? outwardNormal : -outwardNormal; // If ray is coming from inside then flip normal to oppose ray
    }
};

struct hittable
{
    virtual ~hittable() = default;

    virtual bool hit(const ray &r, interval rayT, hitRecord &rec) const = 0;
};
