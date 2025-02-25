#ifndef HITTABLE_H
#define HITTABLE_H 

class hit_record {
    public: 
        point3 p; 
        vec3 normal; 
        double t;
        bool front_face; 

        //sets hit record normal vector so that it always points against the ray direction
        //Assume: outward_normal is normalized
        void set_face_normal(const ray& r, const vec3& outward_normal){
            front_face = dot(r.direction(), outward_normal) < 0; 
            normal = front_face ? outward_normal : -outward_normal; 
        }
};

class hittable {
    public: 
        virtual ~hittable() = default; 
        //hit function taking in a ray & interval [tmin, tmax] that represents valid interval of hits
        virtual bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const = 0;
};

#endif