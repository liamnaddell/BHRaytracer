#include "common.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "img.h"
#include "sphere.h"
#include "ticktock.h"
#include <boost/gil.hpp>
#include <boost/gil/extension/io/jpeg.hpp>
#ifdef IS_MPI
#include <mpi.h>
#endif
#include "decomp.h"
#ifdef _OPENMP
#include <omp.h>
#endif
namespace bg = boost::gil;

struct args {
    //-i img_name
    string img_name = "../squares.jpg";
    //-b <loc>
    double bh_loc = -200.0;
    //-B <bg_loc>
    double bg_loc = -500.0;
    //-M mass
    double mass = 10;
    //-e epsilon
    double epsilon = 2;
    //-W <width>
    int width = 900;
    //-d
    bool debug = false;
    //-s <snum>
    int samples = 1;
    bool wait_for_attach = false;
    //-c <cores>
    int cores = 0;
    //-T
    bool time = false;
};

void help() {
    std::cout << R"(
  -i <img_name>:
    default ../squares.jpg
  -b <black hole loc>
    default -400
  -B <background loc>
    default -500
  -M <mass>
    default 10
  -e <epsilon>
    default 5
  -W <width>
    default 400
  -d
    set debug, default false
  -c <cores>
    use <cores> for multiprocessing.
  -S
    wait for GDB to attach before initializing mpi..
  -s <number of samples>
    default 1)"
              << '\n';
}

bool parse_args(int argc, char *argv[], struct args &as) {
    int c;

    opterr = 0;

    while ((c = getopt(argc, argv, "db:B:e:i:M:s:Sc:W:T")) != -1)
        switch (c) {
        case 'd':
            as.debug = true;
            break;
        case 'T':
            as.time = true;
            break;
        case 'S':
            as.wait_for_attach = true;
            break;
        case 'b':
            as.bh_loc = atof(optarg);
            break;
        case 's':
            as.samples = atoi(optarg);
            break;
        case 'W':
            as.width = atoi(optarg);
            break;
        case 'c':
            as.cores = atoi(optarg);
            break;
        case 'B':
            as.bg_loc = atof(optarg);
            break;
        case 'M':
            as.mass = atof(optarg);
            break;
        case 'e':
            as.epsilon = atof(optarg);
            break;
        case 'i':
            as.img_name = string(optarg);
            break;
        case '?':
            if (optopt == 'c' || optopt == 'b' || optopt == 'B' ||
                optopt == 'M' || optopt == 'e' || optopt == 's' ||
                optopt == 'i') {
                fprintf(stderr, "Option -%c requires an argument.\n", optopt);
            } else if (isprint(optopt))
                fprintf(stderr, "Unknown option `-%c'.\n", optopt);
            else
                fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
            help();
            return false;
        default:
            abort();
        }
    return true;
}

// Wanted/needed CLI arguments:
// 1. bh location on the z axis
// 2. bg location on the z axis
// 3. bg filename (default: stars.jpg)
// 4. samples per pixel (default: 1)
// 5. epsilon value (default: 5)
// 6. max steps (default: 2000 / epsilon)
// 7. single ray debug mode.
int main(int argc, char *argv[]) {
    hittable_list world;
    int x, y;
    bool single_ray = false;
    struct args as;
    // janky hack
    if (argc == 3 && argv[1][0] != '-') {
        single_ray = true;
        x = atoi(argv[1]);
        y = atoi(argv[2]);
    } else {
        bool good = parse_args(argc, argv, as);
        if (!good) {
            puts("Arg parse failed");
            return -1;
        }
	
        std::cerr << "Image name: " <<as.img_name <<
        "\nbh_loc: "<<as.bh_loc <<
        "\nbg_loc: "<<as.bg_loc <<
        "\nmass: "<<as.mass <<
        "\nepsilon: "<<as.epsilon <<
        "\nwidth: "<<as.width <<
        "\ndebug: "<<as.debug <<
        "\nsamples: "<<as.samples << "\n";
    }
    TickTock tt;
    tt.tick();

    if (as.wait_for_attach) {
        volatile int i = 0;
        char hostname[256];
        gethostname(hostname, sizeof(hostname));
        printf("PID %d on %s ready for attach\n", getpid(), hostname);
        fflush(stdout);
        while (0 == i)
            sleep(5);
    }

#ifdef _OPENMP
    if (as.cores != 0) {
        omp_set_num_threads(as.cores);
    }
#endif

    world.add(make_shared<img>(as.img_name.c_str(), as.bg_loc));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = as.width;
    cam.samples_per_pixel = as.samples;
    cam.setup_hole(point3(0, 0, as.bh_loc), as.mass, as.epsilon);
    cam.set_debug(as.debug);
    int width = as.width;
    int height = int(width / cam.aspect_ratio);

    if (single_ray) {
        cam.debug_ray(world, x, y);
        return 0;
    }

#ifdef IS_MPI
    int proc_num;
    int num_procs;
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    proc_num = world_rank;
    num_procs = world_size;
#else
    int proc_num = 0;
    int num_procs = 1;
#endif

    decomp dc(height, width, num_procs);
    dc.perform_decomp();
    char *pxs;
    // 3 bytes per pixel.
    if (proc_num != 0) {
        pxs = (char *)malloc(dc.get_recvcounts()[proc_num]);
    } else {
        pxs = (char *)malloc(3 * width * height);
    }
    assert(pxs != nullptr);
    cam.render(world, pxs, proc_num, dc.get_core_info(proc_num));

#ifdef IS_MPI
    const int *recvcounts = dc.get_recvcounts();
    const int *displs = dc.get_displs();
    int r = MPI_Gatherv(pxs, recvcounts[proc_num], MPI_CHAR, pxs, recvcounts,
                        displs, MPI_CHAR, 0, MPI_COMM_WORLD);
    assert(r == MPI_SUCCESS);
#endif

    if (proc_num == 0) {
        auto out_view = bg::interleaved_view(
            width, height, reinterpret_cast<bg::rgb8_pixel_t *>(pxs),
            width * 3);

        bg::write_view("img.jpg", out_view, bg::jpeg_tag());
    }

    if (as.time) {
        tt.tock("tock");
    }

#ifdef IS_MPI
    // Finalize the MPI environment.
    MPI_Finalize();
#endif
}
