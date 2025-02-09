#include "common.h"
#include "solve.h"
#include <cassert>
#include <fstream>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv2.h>
#include <stdio.h>

int main(void) {
    // new plan: rotate according to start point and direction until we have
    // CONSTANT z, then perform a y rotation to set z to zero. Then un-rotate at
    // the end.
    double epsilon = 1;
    int max = 1000;
    point3 origin(0, 0, 0), direction(-0.75, 0.46, -0.46);
    point3 bh_loc(0, 0, -500);

    ray lr(origin, direction);
    ray_iterator ri(10, lr, bh_loc, epsilon, true, false);

    std::ofstream outr("out_real.txt");
    std::ofstream oute("out_expected.txt");
    ray dr = lr;
    for (;;) {
        puts("iter");
        if (max == 0) {
            break;
        }
        bool good = ri.iter(&dr);
        lr.orig += lr.dir * epsilon;
        if (!good) {
            fprintf(stderr, "DIED\n");
            break;
        }

        point3 pos = dr.at(0);
        // outr << format("%.5e %.5e %.5e\n", pos.x(),pos.y(),pos.z());
        outr << pos.x() << " " << pos.y() << " " << pos.z() << "\n";
        pos = lr.at(0);
        oute << pos.x() << " " << pos.y() << " " << pos.z() << "\n";
        max--;
    }
    return 0;

#if 0
  double x_rot_param = find_x_rot(direction_real);
  point3 direction = perform_x_rot(direction_real,x_rot_param);
  point3 start_pt = perform_x_rot(start_real,x_rot_param);

  double y_rot_param = find_y_rot(start_pt);
  start_pt = perform_y_rot(start_pt,y_rot_param);

  auto unrotate = [=](point3 pt) {
    return perform_x_rot(perform_y_rot(pt,-1 * y_rot_param),-1 * x_rot_param);
  };
  ray r = ray(start_pt,direction);


  point3 start = r.at(0);
  assert(fabs(start.z()) < 0.000001);
  assert(fabs(direction.z()) < 0.000001);
  point3 next = r.at(EPSILON);
  spher3 s_start = to_spher3(start);
  spher3 s_next = to_spher3(next);

  double initial_r = s_start.x();
  double next_r = s_next.x();
  double initial_theta= s_start.y();
  double initial_phi = s_start.z();
  double next_phi = s_next.z();

  //difference in u with respect to rho
  //double initial_velo = (1/next_r - 1/initial_r)/(next_phi - initial_phi) - 0.745;
  double initial_velo = (1/next_r - 1/initial_r)/(next_phi - initial_phi);
  assert(2 * initial_velo != initial_velo);
  //double initial_velo = (1/next_r - 1/initial_r);

  double mass = 0.79;

  gsl_odeiv2_system sys = {func, jac, 2, &mass};

  gsl_odeiv2_driver * d = gsl_odeiv2_driver_alloc_y_new (&sys, gsl_odeiv2_step_rk4, 1e-4, 1e-4, 0.0);

  int i;
  double t = next_phi;
  if (t < 0) 
    t += 2*pi;
  double y[2] = { 1/next_r,initial_velo};

  //TODO: Check conversion in/out functions
  //TODO: Fix this to use new coords!!
  //printf ("%.5e %.5e %.5e\n", start.x(),start.y(),start.z());
  next = unrotate(next);
  printf ("%.5e %.5e %.5e\n", next.x(),next.y(),next.z());
  double new_phi;
  double G = 6.6743;
  double rs = 2 * G * mass;
  fprintf (stderr,"rs=%lf\n", rs);
  for (;;)
    {
      point3 pos=r.at(0);
      spher3 s_pos = to_spher3(pos);
      point3 next_pos=r.at(EPSILON);
      spher3 s_next_pos=to_spher3(next_pos);
      new_phi = s_next_pos.z();

      if (new_phi < 0) {
        new_phi += 2*pi;
        //assert(false);
      }
      int status = gsl_odeiv2_driver_apply (d, &t, new_phi, y);

      if (status != GSL_SUCCESS)
        {
          fprintf (stderr,"error, return value=%d\n", status);
          break;
        }

      if (1/y[0] <= rs) {
        fprintf(stderr,"succ\n");
        break;
      }

      if (1/y[0] > 200) {
        fprintf(stderr,"freedom\n");
        break;
      }

      //for computing theta, we know that pos, next_pos, and iter_pos all lie in the same plane, since BH is invariant under angle of approach.
      //so we can linearly extrapolate theta.

      fprintf (stderr,"spher: r=%.5e,theta=%.5e, phi=%.5e\n", 1/y[0],s_next_pos.y(),t);
      point3 iter_pos = from_spher3(spher3(1/y[0],s_next_pos.y(),t));
      point3 iter_pos_wz=unrotate(iter_pos);
      point3 new_direction = unit_vector(iter_pos - pos);
      r = ray(iter_pos,new_direction);
      printf ("%.5e %.5e %.5e\n", iter_pos_wz.x(),iter_pos_wz.y(),iter_pos_wz.z());
      fflush(stdout);
    }

  gsl_odeiv2_driver_free (d);
  return 0;
#endif
}
