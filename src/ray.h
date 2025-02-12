#ifndef RAY_H
#define RAY_H

#include "vec3.h"

struct ray {
  public:
    ray() {}
    /**
     * @param origin Where the ray begins
     * @param direction Where the ray points to
     */
    ray(const point3 &origin, const vec3 &direction)
        : orig(origin), dir(direction) {}
    /** Getter for origin */
    const point3 &origin() const { return orig; }
    /** Getter for direction */
    const vec3 &direction() const { return dir; }

    /** The point the ray is at after traveling for t time */
    point3 at(double t) const { return orig + t * dir; }
    /** Represents the ray as a formatted string */
    string fmt() {
        stringstream s;
        s << "origin: " << orig.fmt() << ", direction: " << dir.fmt();
        return s.str();
    }

    /** The origin of the ray */
    point3 orig;
    /** The direction of the ray */
    vec3 dir;
};

#endif
