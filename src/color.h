#ifndef COLOR_H
#define COLOR_H

#include "common.h"
#include "interval.h"
#include "vec3.h"

using color = vec3;

static inline color to_color(uint8_t r, uint8_t g, uint8_t b) {
    color c(r / 255.0, g / 255.0, b / 255.0);
    return c;
}

static inline void write_color(char *px, const color &pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // translate the [0,1] component values to byte range [0,255]
    static const interval intensity(0.000, 0.999);
    char rbyte = int(256 * intensity.clamp(r));
    char gbyte = int(256 * intensity.clamp(g));
    char bbyte = int(256 * intensity.clamp(b));

    // Write out pixel color components
    // out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
    px[0] = rbyte;
    px[1] = gbyte;
    px[2] = bbyte;
}

#endif
