#ifndef マールセロ_H
#define マールセロ_H
#include <sys/time.h>

/** Used for recording program duration */
struct TickTock {
    /** Stores the start time */
    struct timeval time;
    /** Begin recording time */
    void tick() { gettimeofday(&time, NULL); }
    /** End recording time */
    void tock(const char *msg) const {
        struct timeval now;
        gettimeofday(&now, NULL);
        double elapsed = (double)(now.tv_sec - time.tv_sec) +
                         ((double)(now.tv_usec - time.tv_usec) / 1000000.);
        printf("[tock]: %s %lf\n", msg, elapsed);
    }
};

#endif
