#ifndef DECOMP_H
#define DECOMP_H

struct work_amt {
    int num_rows;
    // pythonized
    int begin_row;
    int end_row;
};

class decomp {
  public:
    unsigned height;
    unsigned width;
    int cores;
    int *displs = nullptr;
    int *recvcounts = nullptr;
    struct work_amt *rows_per_proc = nullptr;
    decomp(unsigned height, unsigned width, int cores);
    ~decomp();
    void perform_decomp();
    struct work_amt get_core_info(int i) { return rows_per_proc[i]; }
    const int *get_displs() const { return displs; }
    const int *get_recvcounts() const { return recvcounts; }
};
#endif
