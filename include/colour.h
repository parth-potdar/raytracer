// color header file - contains function to color in one pixel
#ifndef COLOUR_H
#define COLOUR_H

#include "interval.h"
#include "vec3.h"

using colour = vec3; // alias for vec3 -> color to define a RGB vector

void write_color(std::ostream& out, const colour& pixel_colour){
    auto r = pixel_colour.x();
    auto g = pixel_colour.y();
    auto b = pixel_colour.z();

    // get colour values in 0-255 (1 byte per channel)
    // clamp values between 0 and 1 
    static const interval intensity(0.000, 0.999); // define 0-1 range with some leeway for floating error
    int rbyte = int(255.999 * intensity.clamp(r));
    int gbyte = int(255.999 * intensity.clamp(g));
    int bbyte = int(255.999 * intensity.clamp(b));

   out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif