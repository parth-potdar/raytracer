// color header file - contains function to color in one pixel
#ifndef COLOUR_H
#define COLOUR_H

#include "vec3.h"

#include <iostream>

using colour = vec3; // alias for vec3 -> color to define a RGB vector

void write_color(std::ostream& out, const colour& pixel_colour){
    auto r = pixel_colour.x();
    auto g = pixel_colour.y();
    auto b = pixel_colour.z();

    // get colour values in 0-255 (1 byte per channel)
    int rbyte = int(255.999 * r);
    int bbyte = int(255.999 * g);
    int gbyte = int(255.999 * b);

   out << rbyte << ' ' << bbyte << ' ' << gbyte << '\n';
}

#endif