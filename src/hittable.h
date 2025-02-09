#ifndef HITTABLE_H
#define HITTABLE_H

#include "color.h"
#include "ray.h"

class hit_record {
  public:
    point3 p;
    vec3 normal;
    double t;
    bool front_face;
    bool color_set = false;
    color col;

    bool has_color() const { return color_set; }
    color get_color() const { return col; }
    void set_color(color c) {
        this->col = c;
        color_set = true;
    }

    void set_face_normal(const ray &r, const vec3 &outward_normal) {
        // Sets the hit record normal vector.
        // NOTE: the parameter `outward_normal` is assumed to have unit length.

        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
  public:
    virtual ~hittable() = default;

    virtual bool hit(const ray &r, interval ray_t, hit_record &rec) const = 0;
};

#endif
