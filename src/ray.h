#ifndef RAY_H
#define RAY_H

#include "vec3.h"

struct ray {
  public:
    ray() {}
    ray(const point3 &origin, const vec3 &direction)
        : orig(origin), dir(direction) {}
    const point3 &origin() const { return orig; }
    const vec3 &direction() const { return dir; }

    point3 at(double t) const { return orig + t * dir; }

    point3 orig;
    vec3 dir;
    string fmt() {
	stringstream s;
	s << "origin: " << orig.fmt() << ", direction: " << dir.fmt();
        return s.str();
    }
};

#endif
