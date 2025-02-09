#include "img_data.h"

color img_data::read_pixel(unsigned x, unsigned y) const {
    // TODO: Assert px is in range.
    if (m_kind == kind::JPEG) {
        // We don't write to the pixels so this is safe.
        bg::rgb8_image_t &bg_img_r =
            const_cast<bg::rgb8_image_t &>(this->jpeg_data);
        auto img_view = bg::view(bg_img_r);
        auto px = img_view(x, y);
        return to_color(px[0], px[1], px[2]);
        // unsigned px = x + y * width;
        // color c = pixels[px];

        // return c;
    } else if (m_kind == kind::PPM) {
        uint8_t *col = raw_data + to_ppm_ofs(x, y);
        return to_color(col[0], col[1], col[2]);
    }
    assert(false);
}
optional<unsigned> img_data::parse_digit(FILE *img_f) {
    unsigned dig = 0;
    char c;
    size_t ret = -1;
    while (ret != 0) {
        ret = fread(&c, sizeof(char), 1, img_f);

        if (ret != 1)
            return none;

        unsigned single_dig = c - '0';

        if (single_dig > 10)
            break;

        dig *= 10;
        dig += single_dig;
    }
    return dig;
}

void img_data::parse_ppm(const char *filename) {
    FILE *img_f = fopen(filename, "r");

    char magic[3];
    size_t amt = fread(magic, sizeof(char), 3, img_f);
    assert(amt == 3);
    assert(magic[0] == 'P' and magic[1] == '6');
    optional<unsigned> l_width = parse_digit(img_f);
    optional<unsigned> l_height = parse_digit(img_f);
    optional<unsigned> max_value = parse_digit(img_f);
    // ignored for now
    (void)max_value;

    assert(l_width and l_height);
    this->width = *l_width;
    this->height = *l_height;

    unsigned size = width * height * 3;
    uint8_t *pixel_data = (uint8_t *)malloc(size * sizeof(char));
    assert(pixel_data);

    size_t res = fread(pixel_data, sizeof(char), size, img_f);
    assert(res == size);
    raw_data = pixel_data;
}

img_data::img_data(const char *filename) {
    m_kind = kind::INVALID;
    if (ends_with(filename, "jpg")) {
        m_kind = kind::JPEG;
        bg::image_read_settings<bg::jpeg_tag> readSettings;
        bg::read_image(filename, jpeg_data, readSettings);

        auto img_view = bg::view(jpeg_data);
        this->width = img_view.dimensions()[0];
        this->height = img_view.dimensions()[1];
        assert(width > 0 && height > 0);
    } else if (ends_with(filename, "ppm")) {
        m_kind = kind::PPM;
        parse_ppm(filename);
    }
    assert(m_kind != kind::INVALID);
}
