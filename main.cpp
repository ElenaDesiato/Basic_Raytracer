#include <iostream> 
#include "color.h"
#include "vec3.h"

int main() {
    // Image 
    int image_width = 256; 
    int image_height = 256; 

    // Render
    /* store image in ppm image format
       write out pixels row by row from top to bottom, left to right
       internal representation: r,b,b represented in [0.0, 1.0] 
       for external representation (ppm): must be scaled to [0,255] */
    /* write image to standard output 
       write progress report to log */
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    
    for (int j = 0; j < image_height; j++) {
        //logging
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; i++) {
            // this is just an example image being displayed
            auto pixel_color = color(double(i)/(image_width-1),double(j)/(image_height-1),0);
            write_color(std::cout,pixel_color);
        }
    }
    std::clog << "\rDone.                 \n";
}