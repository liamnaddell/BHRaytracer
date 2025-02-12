#ifndef HITTABLE_H
#define HITTABLE_H

#include "color.h"
#include "ray.h"

/**
 * Used for describing when a cast light ray hits an object in the scene
 */
class hit_record {
  public:
    /** Where the ray hit */
    point3 p;
    /** The surface normal of intersection when a ray hits a sphere */
    vec3 normal;
    /**
     * The ray starts with an initial position and velocity vector v. The ray
     * hit at point p. p = v * t
     */
    double t;

    /** Is the ray inside or outside the sphere */
    bool front_face;
    /** Used for overriding color -- set by image intersection */
    bool color_set = false;
    /** The associated color */
    color col;

    /** Accessor function for color_set */
    bool has_color() const { return color_set; }
    /** Getter for color */
    color get_color() const { return col; }
    /** Setter for color */
    void set_color(color c) {
        this->col = c;
        color_set = true;
    }

    /** Sets the hit record normal vector and front_face */
    void set_face_normal(const ray &r, const vec3 &outward_normal) {
        // NOTE: the parameter `outward_normal` is assumed to have unit length.

        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

/** Interface for hittable objects */
class hittable {
  public:
    virtual ~hittable() = default;

    /**
     * Did the ray hit our object?
     * @param r The ray in question
     * @param ray_t The span of the ray in question
     * @param rec Out parameter used for recording hit information
     */
    virtual bool hit(const ray &r, interval ray_t, hit_record &rec) const = 0;
};

#endif
