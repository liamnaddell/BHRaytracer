#include "camera.h"
#include "hittable.h"
#include "solve.h"
#include <gsl/gsl_odeiv2.h>
#ifdef _OPENMP
#include <omp.h>
#endif
#include "decomp.h"

void camera::setup_hole(point3 o, double M, double epsilon) {
    this->bh_origin = o;
    this->bh_mass = M;
    this->epsilon = epsilon;
}

void camera::debug_ray(const hittable &world, int i, int j) {
    debug = true;
    initialize();
    assert(i < image_width && j < image_height);

    ray r = get_ray(i, j);
    std::cout << "for ray" << r.fmt();
    color pixel_color = ray_color(r, world);
    std::cout << "COLOR: " << pixel_color.fmt();
}

void camera::render(const hittable &world, char *out_pixels, int proc_num,
                    struct work_amt wa) {
    initialize();

    // divide up the image height using the process number.
    int start_height = wa.begin_row;
    int end_height = wa.end_row;
#pragma omp parallel for default(none)                                         \
    shared(std::clog, std::cout, out_pixels, world)                            \
    firstprivate(start_height, end_height, proc_num)
    for (int j = start_height; j < end_height; j++) {
        // output progress
#if defined(_OPENMP) && !defined(IS_MPI)
        if (omp_get_thread_num() == 0)
            std::clog << "\rScanlines remaining: "
                      << (end_height / omp_get_num_threads() - j) << ' '
                      << std::flush;
#else
        if (proc_num == 0)
            std::clog << "\rScanlines remaining: " << (end_height - j) << ' '
                      << std::flush;
#endif
        char *px = out_pixels + ((j - start_height) * 3 * image_width);
        for (int i = 0; i < image_width; i++) {
            color pixel_color(0, 0, 0);
            // antialiasing
            for (int sample = 0; sample < samples_per_pixel; sample++) {
                ray r = get_ray(i, j);
                if (debug) {
                    std::cout << "for pixel (i,j): (" << i << "," << j << ")\n";
                    std::cout << "for ray " << r.fmt() << '\n';
                }
                pixel_color += ray_color(r, world);
            }
            write_color(px, pixel_samples_scale * pixel_color);
            px += 3;
        }
    }

    std::clog << "\rDone.                 \n";
}

void camera::initialize() {
    image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;
    pixel_samples_scale = 1.0 / samples_per_pixel;

    center = point3(0, 0, 0);

    // Determine viewport dimensions.
    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width =
        viewport_height * (double(image_width) / image_height);

    // Calculate the vectors across the horizontal and down the vertical
    // viewport edges.
    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to
    // pixel.
    pixel_delta_u = viewport_u / image_width;
    pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel.
    auto viewport_upper_left =
        center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
    pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
}

ray camera::get_ray(int i, int j) const {
    // Construct a camera ray originating from the origin and directed at
    // randomly sampled point around the pixel location i, j.

    auto offset = sample_square();
    auto pixel_sample = pixel00_loc + ((i + offset.x()) * pixel_delta_u) +
                        ((j + offset.y()) * pixel_delta_v);

    auto ray_origin = center;
    auto ray_direction = pixel_sample - ray_origin;

    return ray(ray_origin, ray_direction);
}

vec3 camera::sample_square() const {
    // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit
    // square.
    return vec3(random_double() - 0.5, random_double() - 0.5, 0);
}

color camera::ray_color(ray &r, const hittable &world) const {
    hit_record rec;
    point3 pt;
    ray dr;
    double cur = 0.0;
    double max = epsilon * 2000;
    ray_iterator ri(bh_mass, r, bh_origin, epsilon, true, false);

    while (cur < max) {
        if (debug) {
            std::cout << "CURRENT RAY LEN: " << dr.origin().length() << '\n';
        }
        solve_ret stat = ri.iter(&dr);

        if (stat == S_ERROR) {
            fprintf(stderr, "Simulation died after %lf steps\n", cur / epsilon);
            return color(255, 0, 0);
        }

        if (stat == S_SUCC) {
            // fprintf(stderr,"SUCC\n");
            return color(0, 0, 0);
        }

        assert(stat == S_GOOD);

        if (debug) {
            std::cout << dr.fmt() << '\n';
        }

        if (world.hit(dr, interval(0, epsilon), rec)) {
            if (rec.has_color()) {
                return rec.get_color();
            } else {
                return 0.5 * (rec.normal + color(1, 1, 1));
            }
        }
        cur += epsilon;
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5 * (unit_direction.y() + 1.0);
    color c = (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);

    assert(c.x() < 256 && c.y() < 256 && c.z() < 256);
    return c;
}
