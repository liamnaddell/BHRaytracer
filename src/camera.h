#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"

class camera {
  public:
    /** Ratio of image width/height */
    double aspect_ratio = 1.0;
    /** Rendered image width in pixel count */
    int image_width = 100;
    /** Count of random samples for each pixel */
    int samples_per_pixel = 1;
    /** Origin of the black hole */
    point3 bh_origin;
    /** Enable extra debug printing */
    bool debug = true;
    /** Mass of the black hole */
    double bh_mass;
    /** step size for simulation */
    double epsilon;

    /**
     * Configures some supplied black hole parameters
     *
     * @param o Black hole origin
     * @param M Black hole mass
     * @param epsilon simulation step size
     */
    void setup_hole(point3 o, double M, double epsilon);

    /** Enable debugging */
    void set_debug(bool d) { debug = d; }

    /**
     * cast a single ray through pixel i,j in the viewplane for debugging
     * purposes
     */
    void debug_ray(const hittable &world, int i, int j);

    /**
     * Performs the actual image rendering
     *
     * @param world The scene to render
     * @param out_pixels The 8-bit rgb buffer to render into
     * @param proc_num the MPI process number of our process
     * @param wa Used for determining what part of the scene to render
     */
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
