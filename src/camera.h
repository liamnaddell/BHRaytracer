#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"

class camera {
  public:
    /* Public Camera Parameters Here */
    double aspect_ratio = 1.0; /* Ratio of image width over height */
    int image_width = 100;     /* Rendered image width in pixel count */
    int samples_per_pixel = 1; /* Count of random samples for each pixel */
    // origin of black hole
    point3 bh_origin;
    // extra prints
    bool debug = true;
    // mass of black hole
    double bh_mass;
    // step size for simulation
    double epsilon;

    void setup_hole(point3 o, double M, double epsilon);

    void set_debug(bool d) { debug = d; }

    // cast a single ray through pixel i,j in the viewplane for debugging
    // purposes
    void debug_ray(const hittable &world, int i, int j);

    // render entire image to out_pixels.
    void render(const hittable &world, char *out_pixels, int proc_num,
                struct work_amt wa);

  private:
    int image_height;           // Rendered image height
    double pixel_samples_scale; // Color scale factor for a sum of pixel samples
    point3 center;              // Camera center
    point3 pixel00_loc;         // Location of pixel 0, 0
    vec3 pixel_delta_u;         // Offset to pixel to the right
    vec3 pixel_delta_v;         // Offset to pixel below

    void initialize();

    ray get_ray(int i, int j) const;

    vec3 sample_square() const;

    color ray_color(ray &r, const hittable &world) const;
};

#endif
