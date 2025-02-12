#ifndef IMG_DATA_H
#define IMG_DATA_H
#include "color.h"
#include <boost/gil.hpp>
#include <boost/gil/extension/io/jpeg.hpp>

namespace bg = boost::gil;

/**
 * A class which manages an in-memory loaded image
 */
class img_data {
    /** The kind of image loaded */
    enum kind {
        JPEG,
        PPM,
        INVALID,
    };

  public:
    /** The image width */
    unsigned get_width() const { return width; }
    /** The image height */
    unsigned get_height() const { return height; }

  private:
    uint8_t *raw_data;
    bg::rgb8_image_t jpeg_data;
    unsigned width;
    unsigned height;
    kind m_kind;
    /** Used during PPM parsing */
    optional<unsigned> parse_digit(FILE *img_f);
    /** Converts an x and y coordinate into an offset into the image data buffer
     */
    unsigned to_ppm_ofs(unsigned x, unsigned y) const {
        return (y * get_width() + x) * 3;
    }
    void parse_ppm(const char *filename);

  public:
    /** A helper function which reads the pixel value at a particular x and y
     * coordinate */
    color read_pixel(unsigned x, unsigned y) const;

    /**
     * @param filename The filename to load (can either be JPEG or PPM)
     */
    img_data(const char *filename);
};
#endif
