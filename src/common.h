#ifndef COMMON_H
#define COMMON_H

#include <cassert>
#include <cmath>
#include <cstring>
#include <iostream>
#include <limits>
#include <memory>
#include <random>
#include <sstream>
#include <string>

#include <boost/optional.hpp>
using std::make_shared;
using boost::none;
using boost::optional;
using std::shared_ptr;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

inline double random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline bool ends_with(const char *s, const char *ext) {
    // 8
    unsigned slen = strlen(s);
    // 3
    unsigned extlen = strlen(ext);
    if (extlen > slen) {
        return false;
    }
    // 5
    unsigned diff = slen - extlen;
    const char *sext = s + diff;
    return strncmp(sext, ext, extlen) == 0;
}

using std::string;
using std::stringstream;

#endif
