#ifndef VEC3_H 
#define VEC3_H 

class vec3 {
    public: 
        double e[3]; 
        
        vec3() : e{0,0,0} {}
        vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

        double x() const { return e[0]; }
        double y() const { return e[1]; }
        double z() const { return e[2]; }

        vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
        double operator[](int i) const { return e[i]; }
        double& operator[](int i) { return e[i]; }

        vec3& operator+=(const vec3& v) {
            e[0] += v.e[0]; 
            e[1] += v.e[1]; 
            e[2] += v.e[2]; 
            return *this; 
        }
        vec3& operator*=(double t) {
            e[0] *= t;  
            e[1] *= t; 
            e[2] *= t; 
            return *this; 
        }
        vec3& operator/=(double t){
            return *this *= 1/t; 
        }

        double length() const {
            return std::sqrt(length_squared()); 
        }

        double length_squared() const { 
            return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
        }

        //return true iff vector is close to 0 in all dimensions
        bool near_zero() const { 
            auto t = 1e-8; 
            return (std::fabs(e[0]) < t) && (std::fabs(e[1])<t) && (std::fabs(e[2])<t);
        }

        static vec3 random() { 
            return vec3(random_double(), random_double(), random_double());
        }

        static vec3 random(double min, double max){
            return vec3(random_double(min,max), random_double(min,max), random_double(min,max));
        }
    };

// point3 as an alias of vec3 
using point3 = vec3; 
    
//Vector Utility Functions

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double t, const vec3& v) {
    return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline vec3 operator*(const vec3& v, double t) {
    return t * v;
}

inline vec3 operator/(const vec3& v, double t) {
    return (1/t) * v;
}

inline double dot(const vec3& u, const vec3& v) {
    return u.e[0]*v.e[0] + u.e[1]*v.e[1] + u.e[2]*v.e[2];
}

inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(const vec3& v) {
    return v / v.length();
}

// generate a random unit vector by generating vectors of random doubles [-1,1] and rejecting if outside unit sphere (or if due to floating point values too small)
inline vec3 random_unit_vector() {
    while (true){
        auto p = vec3::random(-1,1);
        auto norm_squared = p.length_squared();
        if(1e-160 < norm_squared && norm_squared <= 1) {
            return p/sqrt(norm_squared); 
        }
    }
}

//generate a random unit vector that is also in the hemisphere outside of surface
// can evaluate this by using dot product of surface normal & random vector
inline vec3 random_on_hemisphere(const vec3& normal){
    vec3 unit_vector = random_unit_vector(); 
    if(dot(unit_vector, normal) > 0.0) 
        return unit_vector; 
    else 
        return -unit_vector;
}

//reflect a ray around normal
//assume: normal is normalized, incoming ray pointing towards surface
inline vec3 reflect(const vec3& v, const vec3& n) {
    //project v onto n (n is normalized, so this is dot product) & negate this projection (s.t. b points out of surface)
    return v - 2*dot(v,n)*n;
}

//refract a ray at the boundary of 2 materials
/* using Snell's law: eta*sin(theta) = eta' * sin(theta') where eta, eta' are the refractive indices and theta, theta' the angles from normal
    then solve for sin(theta') and split refracted ray into parts perpendicular and parallel to normal n' */
inline vec3 refract(const vec3& uv, const vec3& n, double eta_ratio) {
    auto cos_theta = std::fmin(dot(-uv,n),1.0); 
    vec3 r_out_perp = eta_ratio*(uv + cos_theta*n); 
    vec3 r_out_par = -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared()))*n; 
    return r_out_perp + r_out_par; 
}

#endif