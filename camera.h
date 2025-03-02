#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"

class camera { 
    public: 
        double aspect_ratio = 1.0; 
        int image_width = 100; 
        int samples_per_pixel = 10; 
        int max_depth = 10; 

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
                    //for antialiasing: select samples around pixel & average
                    color pixel_color(0,0,0);
                    for (int s = 0; s < samples_per_pixel; s++) {
                        ray r = get_ray(i,j);
                        pixel_color += ray_color(r,max_depth, world);
                    }
                    write_color(std::cout,samples_scale*pixel_color);
                }
            }
            std::clog << "\rDone.                 \n";
        }
    
    private: 
        int image_height; 
        double samples_scale; 
        point3 camera_center;    
        point3 pixel00_loc;     //location of pixel 0,0
        vec3 pixel_dist_u;     //offset to pixel to the right
        vec3 pixel_dist_v;     //offset to pixel below

        void initialize() {
            //calculate image height based on desired aspect ratio & given image width
            image_height = int(image_width/aspect_ratio); 
            image_height = (image_height < 1) ? 1 : image_height; 

            //for antialiasing, here simple average of samples
            samples_scale = 1.0/samples_per_pixel; 

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

        //Helper function: return vector to a random point in [-0.5,5]^2 unit square
        vec3 sample_square() const { 
            return vec3(random_double() - 0.5, random_double() - 0.5, 0);
        }
        // Construct a camera ray originating from origin & directed at randomly sampled point around (i,j)
        ray get_ray(int i, int j) const {
            auto offset = sample_square(); 
            auto pixel_sample = pixel00_loc + ((i+offset.x())* pixel_dist_u) + ((j+offset.y()) * pixel_dist_v);

            auto ray_origin = camera_center; 
            auto ray_dir = pixel_sample - ray_origin; 
            return ray(ray_origin, ray_dir);
        }

        // How to color the rays
        // currently: linear interpolation (blend white & blue depending on y after normalization) for background
        //            & treat spheres as diffuse (rays bounce off material equally in all directions); here: keep 50% of color (-> gray)
        color ray_color(const ray& r, int depth, const hittable& world) {
            if (depth <= 0) return color(0, 0, 0);

            hit_record rec; 
            if (world.hit(r,interval(0,infinity),rec)) {
                vec3 dir = random_on_hemisphere(rec.normal);
                return 0.5 * ray_color(ray(rec.p, dir), depth -1,world);
            }

            vec3 unit_dir = unit_vector(r.direction());
            auto a = 0.5 * (unit_dir.y() + 1.0);
            return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
        }
};

#endif