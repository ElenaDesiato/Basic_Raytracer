#ifndef COLOR_H
#define COLOR_H

#include "vec3.h" 
#include "interval.h"

using color = vec3; //an alias of vec3 class

//write out a single pixel for ppm format
void write_color(std::ostream& out, const color& pixel_color){
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    //[0,1] range to [0,255] range
    static const interval intensity (0.000, 0.999);
    int rbyte = int(256 * intensity.clamp(r));
    int gbyte = int(256 * intensity.clamp(g));
    int bbyte = int(256 * intensity.clamp(b));

    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n'; 
}


#endif