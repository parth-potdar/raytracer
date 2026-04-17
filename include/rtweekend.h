// Main header file
#ifndef RTWEEKEND_H
#define RTWEEKEND_H

// include standard libraries
#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <random>

// std usings
using std::make_shared;
using std::shared_ptr;

// define constants
const double infinity = std::numeric_limits<double>::infinity(); // get infinity special bit-pattern
const double pi = 3.1415926535897932385;

// utility functions
inline double degrees_to_radians(double degrees) { // inline means ignore repeat references -> fo this for funcs in headers
    return degrees * pi / 180.0;
}

inline double random_double() {
    // define distribution/range - here it is uniform distribution
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);

    // random number generator - Mersenne Twister
    static std::mt19937 generator;

    return distribution(generator);
}

inline double random_double(double min, double max) {
    // variant of random_double if min and max range provided
    // return a random real number in the range by adding some random fraction of max to min value
    return min + max*random_double();
}

// include the raytracing headers
#include "colour.h"
#include "ray.h"
#include "vec3.h"
#include "interval.h"

#endif