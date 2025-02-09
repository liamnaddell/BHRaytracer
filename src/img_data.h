#ifndef IMG_DATA_H
#define IMG_DATA_H
#include "color.h"
#include <boost/gil.hpp>
#include <boost/gil/extension/io/jpeg.hpp>

namespace bg = boost::gil;

class img_data {
    enum kind {
        JPEG,
        PPM,
        INVALID,
    };

  private:
    uint8_t *raw_data;
    bg::rgb8_image_t jpeg_data;
    unsigned width;
    unsigned height;
    kind m_kind;

  public:
    unsigned get_width() const { return width; }
    unsigned get_height() const { return height; }
    unsigned to_ppm_ofs(unsigned x, unsigned y) const {
        return (y * get_width() + x) * 3;
    }
    color read_pixel(unsigned x, unsigned y) const;
    optional<unsigned> parse_digit(FILE *img_f);

    void parse_ppm(const char *filename);

    img_data(const char *filename);
};
#endif
