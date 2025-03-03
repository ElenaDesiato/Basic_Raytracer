#ifndef SPHERE_H 
#define SPHERE_H 

#include "hittable.h"

class sphere : public hittable {
    public: 
        sphere (const point3& center, double radius, shared_ptr<material> mat) : center(center), radius(std::fmax(0,radius)), mat(mat) {}

        /* Sphere intersections 
        * = dot product
        sphere around (C_x, C_y, C_z): (C_x - x)^2 + (C_y - y)^2 + (C_z - z)^2 = r^2   (if < : inside, if > : outside)
        a vector from a point P = (x,y,z) to C = (C_x, C_y, C_z) is C-P
        -> by def. of dot product: sphere equation : (C-P)*(C-P) = r^2

        ray: P(t) = Q + td
        input into sphere equation: (C-(Q+td)) * (C-(Q+td)) = r^2
        <=> solve t^2 d*d - 2td * (C-Q) + (C-Q) * (C-Q) - r^2 = 0 
        <=> a = d*d     b = -2d * (C-Q)     c = (C-Q)*(C-Q) - r^2
        which can have 0,1 or 2 results depending on location of sphere & ray

        Simplification: let b = -2h and simplify midnight formula to (h+- sqrt(h^2-ac))/a 
        Solving for h: b = -2d * (C-Q) = -2h        h = b/(-2) = d*(C-Q)
        */
        bool hit(const ray& r, interval ray_t, hit_record& rec) const override { 
            vec3 cq = center - r.origin(); 
            auto a = r.direction().length_squared();
            auto h = dot(r.direction(),cq); 
            auto c = cq.length_squared() - radius*radius; 
            auto discriminant = h*h - a*c; 

            if (discriminant < 0) 
                return false; 
            auto sqrtd = std::sqrt(discriminant); 

            //find nearest root within acceptable range [tmin,tmax]
            auto root = (h-sqrtd)/a;
            if (!ray_t.contains(root)) { 
                root = (h+sqrtd)/a; 
                if (!ray_t.contains(root)) 
                    return false; 
            }
            rec.t = root; 
            rec.p = r.getPoint(rec.t); 
            //for sphere: normal = hit point - center, normalized by dividing by radius
            vec3 outward_normal = (rec.p - center)/radius; 
            rec.set_face_normal(r,outward_normal);
            rec.mat = mat; 
            return true; 
        }
    
    private: 
        point3 center; 
        double radius; 
        shared_ptr<material> mat; 
};

#endif