#include <iostream> 
#include "color.h"
#include "vec3.h"
#include "ray.h"

// Calculating intersections of ray with sphere
/* 
    * = dot product

    sphere around (C_x, C_y, C_z): (C_x - x)^2 + (C_y - y)^2 + (C_z - z)^2 = r^2   (if < : inside, if > : outside)
    a vector from a point P = (x,y,z) to C = (C_x, C_y, C_z) is C-P
    -> by def. of dot product: sphere equation : (C-P)*(C-P) = r^2

    ray: P(t) = Q + td
    input into sphere equation: (C-(Q+td)) * (C-(Q+td)) = r^2
    <=> solve t^2 d*d - 2td * (C-Q) + (C-Q) * (C-Q) - r^2 = 0 
    <=> a = d*d     b = -2d * (C-Q)     c = (C-Q)*(C-Q) - r^2
    which can have 0,1 or 2 results depending on location of sphere & ray
*/
double hit_sphere (const point3& center, double radius, const ray& r) {
    vec3 cq = center - r.origin();
    auto a = dot(r.direction(), r.direction()); 
    auto b = -2.0* dot(r.direction(), cq); 
    auto c = dot(cq, cq) - radius*radius;
    auto discriminant = b*b - 4*a*c; 

    if (discriminant < 0) {
        return -1.0; 
    }
    else {
        //return t of intersection point
        return (-b - std::sqrt(discriminant))/(2.0*a);
    }
}

// How to color the rays
// currently: linear interpolation (blend white & blue depending on y after normalization) for background
//            & visualize surface normals for sphere
color ray_color(const ray& r) {
    auto t = hit_sphere(point3(0,0,-1),0.5,r);
    if (t > 0.0) {
        /* compute normal (for sphere, this is hit point P minus center C)
           & visualize normals (assume unit normals (in [-1,1]), map each component of normal to [0,1] & treat as rgb)
        */
       vec3 N = unit_vector(r.getPoint(t) - vec3(0,0,-1)); 
       return 0.5*color(N.x() + 1, N.y()+1,N.z()+1);
    }

    vec3 unit_dir = unit_vector(r.direction());
    auto a = 0.5*(unit_dir.y() + 1.0);
    return (1.0 - a)*color(1.0,1.0,1.0) + a*color(0.5,0.7,1.0);
}


int main() {
    // Image 
    auto aspect_ratio = 16.0/9.0; //ideal ratio
    int image_width = 400; 
    //calculate image height based on desired aspect ratio & given image width
    int image_height = int(image_width/aspect_ratio); 
    image_height = (image_height < 1) ? 1 : image_height; 

    //Camera
    //calculate viewport (based on set viewport height) which is rectangle in 3D that contains grid of image pixels
    auto viewport_height = 2.0; 
    auto viewport_width = viewport_height * (double(image_width)/image_height);
    // define camera center as where all rays originate where camera center->viewpoint center is orthogonal to viewport
    // & camera is focal length away from viewport
    auto focal_length = 1.0; 
    auto camera_center = point3(0,0,0);

    //compute some vectors for convenience
    auto viewport_u = vec3(viewport_width,0,0); //entire horiz. viewport edge
    auto viewport_v = vec3(0,-viewport_height,0); //entire vertic. viewport edge

    auto pixel_dist_u = viewport_u/image_width; //horiz. dist. one pixel
    auto pixel_dist_v = viewport_v/image_height; //vertic. dist. one pixel

    //calculate location of upper left pixel
    auto viewport_upper_left = camera_center - vec3(0,0,focal_length) - viewport_u/2 - viewport_v/2; 
    auto pixel00_loc = viewport_upper_left + 0.5*(pixel_dist_u + pixel_dist_v);

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
            auto pixel_center = pixel00_loc + (i*pixel_dist_u)+(j*pixel_dist_v);
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction); 
            color pixel_color = ray_color(r);
            write_color(std::cout,pixel_color);
        }
    }
    std::clog << "\rDone.                 \n";
}