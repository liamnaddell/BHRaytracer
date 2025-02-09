#ifndef IMG_H
#define IMG_H
#include "hittable.h"
#include "img_data.h"
#include <cassert>

class img : public hittable {
  public:
    bool hit(const ray &r, interval ray_t, hit_record &rec) const override;
    img(const char *filename, double z_loc);

  private:
    unsigned loc;
    interval x_range;
    interval y_range;
    interval z_range;
    point3 origin;
    // normal vector to image plane
    vec3 normal;
    std::vector<color> pixels;
    img_data bg_img;
    unsigned height;
    unsigned width;
};
#endif
