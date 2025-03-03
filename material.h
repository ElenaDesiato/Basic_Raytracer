#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h" 

class material {
    public: 
        virtual ~material() = default; 

        virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const
       {(void)r_in; (void)rec; (void)attenuation; (void)scattered; return false; }
};

class lambertian : public material {
    public: 
        lambertian(const color& albedo) : albedo(albedo){} //albedo: a form of fractional reflectance

        //simulates reflection on a diffuse material (always randomly scatter)
        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
            (void) r_in;
            auto scatter_dir = rec.normal + random_unit_vector(); 
            //Catch degenerate scatter direction (if normal is opposite to random unit vector)
            if(scatter_dir.near_zero())
                scatter_dir = rec.normal; 
            scattered = ray(rec.p, scatter_dir); 
            attenuation = albedo; 
            return true; 
        }
    private: 
        color albedo; 
};

class metal : public material {
    public: 
        metal(const color& albedo) : albedo(albedo) {} 

         //simulates reflection on a metal material (reflect around normal)
        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
            vec3 reflected = reflect(r_in.direction(), rec.normal);
            scattered = ray(rec.p,reflected); 
            attenuation = albedo; 
            return true; 
        }
    private: 
        color albedo;
};

#endif