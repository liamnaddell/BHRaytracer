#ifndef SOLVE_H
#define SOLVE_H

#include "common.h"
#include "ray.h"
#include "vec3.h"
#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv2.h>

/** The possible return values from a solution attempt */
enum solve_ret {
    S_GOOD,
    S_ERROR,
    S_SUCC,
};

/** A struct which owns a ray as it travels through space, applying
 * gravitational distortion */
struct ray_iterator {
    /** BH mass */
    double mass;
    /** schwartzschield radius */
    double rs;
    /** BH origin */
    point3 origin;
    /** Step size */
    double epsilon;
    /** Debugging parameter used for `tgsl` */
    bool freedom;
    /** Debugging parameter used for `tgsl` */
    bool disable_bh;

    /** Convert the state of the iterator to a string for debugging */
    string fmt();

    /** March the ray forwards, returns the updated ray as an out parameter */
    solve_ret iter(ray *r);

    /**
     * @param mass The mass of the black hole
     * @param initial_ray The initial light ray which will become distorted
     * @param origin The origin of the black hole
     * @param epsilon The epsilon of the simulation
     * @param prevent_freedom Used for `tgsl` to stop the ray once it is out of
     * a certain range
     * @param disable_bh Another parameter for `tgsl` which disables distortion
     */
    ray_iterator(double mass, ray initial_ray, point3 origin, double epsilon,
                 bool prevent_freedom, bool disable_bh);

    /** Dtor */
    ~ray_iterator() { gsl_odeiv2_driver_free(m_d); }

  private:
    point3 transfer_out(point3 pt);
    double m_z_rot;
    ray m_r;                 /* Current ray */
    double m_t;              /* alias for current phi */
    double m_y[2];           /* (u,phi) */
    gsl_odeiv2_system m_sys; /*= {func, jac, 2, &mass}; */
    gsl_odeiv2_driver *m_d;  /* gsl_odeiv2_driver_apply (d, &t, new_phi, y); */
};

#endif
