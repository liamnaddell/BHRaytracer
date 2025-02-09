#ifndef SOLVE_H
#define SOLVE_H

#include "common.h"
#include "ray.h"
#include "vec3.h"
#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv2.h>

enum solve_ret {
    S_GOOD,
    S_ERROR,
    S_SUCC,
};

struct ray_iterator {
    double mass;    /* BH mass */
    double rs;      /* schwartzschield radius */
    point3 origin;  /* BH origin */
    double epsilon; /* Step size */
    bool freedom;
    bool disable_bh;

    point3 transfer_out(point3 pt);

    string fmt();

    solve_ret iter(ray *r);

    ray_iterator(double mass, ray initial_ray, point3 origin, double epsilon,
                 bool prevent_freedom, bool disable_bh);

    ~ray_iterator() { gsl_odeiv2_driver_free(m_d); }

  private:
    double m_z_rot;
    ray m_r;                 /* Current ray */
    double m_t;              /* alias for current phi */
    double m_y[2];           /* (u,phi) */
    gsl_odeiv2_system m_sys; /*= {func, jac, 2, &mass}; */
    gsl_odeiv2_driver *m_d;  /* gsl_odeiv2_driver_apply (d, &t, new_phi, y); */
};

#endif
