#ifndef INTERVAL_H
#define INTERVAL_H

#include "common.h"

class interval {
  public:
    /** The min of the interval */
    double min;
    /** The max of the interval */
    double max;

    /** Default constructor creates an empty interval */
    interval() : min(+infinity), max(-infinity) {}

    /** Explicit constructor */
    interval(double min, double max) : min(min), max(max) {}

    /** The size of the interval */
    double size() const { return max - min; }

    /** Does the interval contain the value x */
    bool contains(double x) const { return min <= x && x <= max; }

    /** Does the interval strictly contain x */
    bool surrounds(double x) const { return min < x && x < max; }

    /** The nearest value to x inside the interval */
    double clamp(double x) const {
        if (x < min)
            return min;
        if (x > max)
            return max;
        return x;
    }

    /** Helper empty interval */
    static const interval empty;
    /** Helper universe interval which contains all points */
    static const interval universe;
};

#endif
