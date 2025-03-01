#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"

class camera { 
    public: 
        double aspect_ratio = 1.0; 
        int image_width = 100; 

        void render(const hittable& world){ 
            initialize(); 
            /* store image in ppm image format
            write out pixels row by row from top to bottom, left to right
            internal representation: r,b,b represented in [0.0, 1.0] 
            for external representation (ppm): must be scaled to [0,255] */
            /* write image to standard output 
            write progress report to log */
            std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
            
            for (int j = 0; j < image_height; j++) {
                std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
                for (int i = 0; i < image_width; i++) {
                    // this is just an example image being displayed
                    auto pixel_center = pixel00_loc + (i*pixel_dist_u)+(j*pixel_dist_v);
                    auto ray_direction = pixel_center - camera_center;
                    ray r(camera_center, ray_direction); 
                    color pixel_color = ray_color(r, world);
                    write_color(std::cout,pixel_color);
                }
            }
            std::clog << "\rDone.                 \n";
        }
    
    private: 
        int image_height; 
        point3 camera_center;    
        point3 pixel00_loc;     //location of pixel 0,0
        vec3 pixel_dist_u;     //offset to pixel to the right
        vec3 pixel_dist_v;     //offset to pixel below

        void initialize() {
            //calculate image height based on desired aspect ratio & given image width
            image_height = int(image_width/aspect_ratio); 
            image_height = (image_height < 1) ? 1 : image_height; 

            // define camera center as where all rays originate where camera center->viewpoint center is orthogonal to viewport
            // & camera is focal length away from viewport
            auto focal_length = 1.0; 
            camera_center = point3(0,0,0);

            //calculate viewport (based on set viewport height) which is rectangle in 3D that contains grid of image pixels
            auto viewport_height = 2.0; 
            auto viewport_width = viewport_height * (double(image_width)/image_height);

             //compute some vectors for convenience
            auto viewport_u = vec3(viewport_width,0,0); //entire horiz. viewport edge
            auto viewport_v = vec3(0,-viewport_height,0); //entire vertic. viewport edge

            pixel_dist_u = viewport_u/image_width; //horiz. dist. one pixel
            pixel_dist_v = viewport_v/image_height; //vertic. dist. one pixel

            //calculate location of upper left pixel
            auto viewport_upper_left = camera_center - vec3(0,0,focal_length) - viewport_u/2 - viewport_v/2; 
            pixel00_loc = viewport_upper_left + 0.5*(pixel_dist_u + pixel_dist_v);
        }

        // How to color the rays
        // currently: linear interpolation (blend white & blue depending on y after normalization) for background
        //            & visualize surface normals for sphere
        color ray_color(const ray& r, const hittable& world) {
            hit_record rec; 
            if (world.hit(r,interval(0,infinity),rec)) {
                return 0.5*(rec.normal + color(1,1,1));
            }

            vec3 unit_dir = unit_vector(r.direction());
            auto a = 0.5*(unit_dir.y() + 1.0);
            return (1.0 - a)*color(1.0,1.0,1.0) + a*color(0.5,0.7,1.0);
        }
};

#endif