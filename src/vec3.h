#ifndef VEC3_H
#define VEC3_H

#include "common.h"

class vec3 {
  public:
    /** The x,y,z of the vector */
    double e[3];

    /** Default vector is the zero vector */
    vec3() : e{0, 0, 0} {}
    /** Set x,y,z to e0, e1, and e2 */
    vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}
    /** Getter for x */
    double x() const { return e[0]; }
    /** Getter for y */
    double y() const { return e[1]; }
    /** Getter for z */
    double z() const { return e[2]; }

    /** Negate the x,y,z of a vector */
    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    /** Return the ith coordinate of the vector, 0 is x, 1 is y, 2 is z */
    double operator[](int i) const { return e[i]; }
    /** Return a reference to x,y, or z */
    double &operator[](int i) { return e[i]; }

    /** Vector addition */
    vec3 &operator+=(const vec3 &v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }
    /** String formatting for debug output */
    string fmt() {
        stringstream s;
        s << "(" << e[0] << "," << e[1] << "," << e[2] << ")";
        return s.str();
    }

    /** Scalar-vector multiplication */
    vec3 &operator*=(double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    /** Scalar-vector division */
    vec3 &operator/=(double t) { return *this *= 1 / t; }

    /** The euclidean length of a vector */
    double length() const { return std::sqrt(length_squared()); }

    /** The squared length of a vector */
    double length_squared() const {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    /** Convert a vector in spherical coordinates to cartesian coordinates */
    vec3 to_cartesian() {
        double r = this->x();
        double t = this->y();
        double p = this->z();
        return vec3(r * sin(t) * cos(p), r * sin(t) * sin(p), r * cos(t));
    }

    /** Convert a vector in cartesian coordinates to spherical coordinates */
    vec3 to_spher3() {
        double x = this->x();
        double y = this->y();
        double z = this->z();
        double r = sqrt(x * x + y * y + z * z);
        if (fabs(r) == 0.0) {
            return vec3(0, 0, 0);
        }

        double phi;
        // stupid case for atan2(100,-0.0) returning pi/2 instead of 0
        if (fabs(x) == 0.0) {
            phi = 0.0;
            phi = atan2(y, x);
        } else {
            phi = atan2(y, x);
        }

        double theta = acos(z / r);

        // nan
        assert(theta == theta);
        assert(phi == phi);

        return vec3(r, theta, phi);
    }

    /** Rotate a vector about the y axis by theta */
    vec3 roty(double theta) {
        double x = this->x();
        double y = this->y();
        double z = this->z();

        return vec3(x * cos(theta) + z * sin(theta), y,
                    -1 * x * sin(theta) + z * cos(theta));
    }
    /** Rotate a vector about the z axis by theta */
    vec3 rotz(double theta) {
        double x = this->x();
        double y = this->y();
        double z = this->z();

        // x\cos\theta-y\sin\theta
        // x\sin\theta+y\cos\theta
        // z
        return vec3(x * cos(theta) - y * sin(theta),
                    x * sin(theta) + y * cos(theta), z);
    }
    /** Rotate a vector about the x axis by theta */
    vec3 rotx(double theta) {
        double x = this->x();
        double y = this->y();
        double z = this->z();

        return vec3(x, y * cos(theta) + -1 * z * sin(theta),
                    y * sin(theta) + z * cos(theta));
    }
};

// point3 is just an alias for vec3, but useful for geometric clarity in the
// code.
using point3 = vec3;
// spher3 is (r,θ,φ), where θ, φ are polar angle and azimuthal angle.
// note, x-axis is left/right, z axis is viewing direction, y axis is up/down
// for camera. θ can compute displacement along the y axis with respect to hole
// origin r,φ can compute displacement along the xz plane with respect to hole
// origin.
using spher3 = vec3;

// Vector Utility Functions

inline std::ostream &operator<<(std::ostream &out, const vec3 &v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double t, const vec3 &v) {
    return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline vec3 operator*(const vec3 &v, double t) { return t * v; }

inline vec3 operator/(const vec3 &v, double t) { return (1 / t) * v; }

inline double dot(const vec3 &u, const vec3 &v) {
    return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3 &u, const vec3 &v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(const vec3 &v) {
    // nan
    assert(v.length() != 0);
    return v / v.length();
}

#endif
