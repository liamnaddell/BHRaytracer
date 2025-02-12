#include "decomp.h"
#include "common.h"

decomp::decomp(unsigned height, unsigned width, int cores)
    : height(height), width(width), cores(cores) {
    recvcounts = (int *)calloc(sizeof(int), cores);
    assert(recvcounts);
    displs = (int *)calloc(sizeof(int), cores);
    assert(displs);
    rows_per_proc = (struct work_amt *)calloc(sizeof(*rows_per_proc), cores);
    assert(rows_per_proc);
}

decomp::~decomp() {
    free(recvcounts);
    free(displs);
    free(rows_per_proc);
}

void decomp::perform_decomp() {
    // step 1.1: Assign a base number of rows
    int stride = height / cores;

    for (int i = 0; i < cores; i++)
        rows_per_proc[i].num_rows = stride;

    // step 1.2: assign leftover rows
    int leftover_rows = height - (stride * cores);
    assert(leftover_rows < cores);

    for (int i = 0; i < leftover_rows; i++)
        rows_per_proc[i].num_rows += 1;

    // step 2: assign specific rows.
    int current_row = 0;
    int previous_row = 0;
    for (int i = 0; i < cores; i++) {
        current_row += rows_per_proc[i].num_rows;
        rows_per_proc[i].begin_row = previous_row;
        rows_per_proc[i].end_row = current_row;
        previous_row = current_row;
    }
    assert(current_row == (int)height);

    // step 3: build the displs array
    // displs is where each process will write it's image data to in the output
    // file
    for (int i = 0; i < cores; i++)
        // 3 chars for r, g, and b, and 'width' pixels per row.
        displs[i] = rows_per_proc[i].begin_row * 3 * width;

    // step 4: build the recvcounts array
    // recvcounts is how much data each process must store for it's output
    // pixels;
    for (int i = 0; i < cores; i++)
        // again, 3 chars for r,g,b, and 'width' pixels per row
        recvcounts[i] = rows_per_proc[i].num_rows * 3 * width;

    // done!
}
