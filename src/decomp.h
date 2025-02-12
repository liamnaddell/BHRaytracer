#ifndef DECOMP_H
#define DECOMP_H

/**
 * Used for image decomposition -- tells the raytracer where in the image to
 * render
 */
struct work_amt {
    /** The amount of rows to render */
    int num_rows;
    /** Which row to begin rendering at */
    int begin_row;
    /** Which row to stop rendering at (pythonized array indexing) */
    int end_row;
};

class decomp {
    unsigned height;
    unsigned width;
    int cores;
    int *displs = nullptr;
    int *recvcounts = nullptr;
    struct work_amt *rows_per_proc = nullptr;

  public:
    /**
     * @param height the height of the image to decompose
     * @param width the width of the image to decompose
     * @param cores the number of available workers for decomposition
     */
    decomp(unsigned height, unsigned width, int cores);
    /**
     * dtor
     */
    ~decomp();
    /** Perform the actual image decomposition */
    void perform_decomp();
    /** Get the work order required for a particular core */
    struct work_amt get_core_info(int i) { return rows_per_proc[i]; }
    /**
     * This array is built for use with MPI_Gatherv.
     * We have each MPI process render its portion of the image independently.
     *  Afterwards, each process will send the image data across the network to
     * the root node. This root node needs to know what offset into the image
     * buffer a particular process' rendered data needs to go. This array
     * describes that information.
     */
    const int *get_displs() const { return displs; }
    /**
     * Another array built for MPI_Gatherv. This array expresses how much data
     * is sent by each process to the root node
     */
    const int *get_recvcounts() const { return recvcounts; }
};
#endif
