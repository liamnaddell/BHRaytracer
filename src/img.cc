#include "img.h"

bool img::hit(const ray &r, interval ray_t, hit_record &rec) const {
    (void)ray_t;
    (void)rec;

    point3 l0 = r.at(0);
    vec3 l = r.direction();
    point3 p0 = origin;
    vec3 n = normal;

    double d = dot(p0 - l0, n) / dot(l, n);
    // somehow this ray passed through the image without hitting!

    point3 insn = l0 + l * d;
    assert(insn.x() == insn.x());

    if (!ray_t.contains(d)) {
        return false;
    }

    if (x_range.contains(insn.x()) && y_range.contains(insn.y()) &&
        z_range.contains(insn.z())) {
        // we need to convert 3d point to 2d point on image.
        // compute vector from top left to intersection point
        // Just take x and y components.
        // then find row/column in PPM data
        rec.p = insn;
        // rec.normal = normal;
        rec.t = d;
        // rec.front_face = true;
        int x = fabs(origin.x() - insn.x());
        int y = fabs(origin.y() - insn.y());
        rec.set_color(bg_img.read_pixel(x, y));
        return true;
    }
    return false;
}
img::img(const char *filename, double z_loc) : bg_img(filename) {
    vec3 normal(0, 0, 1);

    this->width = bg_img.get_width();
    this->height = bg_img.get_height();
    // center image
    point3 origin(-1 * double(this->width) / 2, -1 * double(this->height) / 2,
                  z_loc);

    this->normal = normal;
    this->x_range = interval(origin.x(), origin.x() + this->width);
    this->y_range = interval(origin.y(), origin.y() + this->height);
    this->z_range = interval::universe;
    this->origin = origin;
}
