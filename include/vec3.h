// Header file defining the vec3 class
// vec3: 3D vectory class

// header guard to prevent multiple inclusions of the same header file
// using if/endif with preprocesser directive #define
#ifndef VEC3_H
#define VEC3_H

// define a 3d vector class
class vec3{
    public: // define public attr and methods of 
        // 3d vector is a double array
        double e[3];

        // define constructors (initialisers of objects)
        vec3() : e{0,0,0} {} // initialises 0 vector if no elements given

        vec3(double e0, double e1, double e2) : e{e0, e1, e2} {} // elems provided

        // define accessors - lets you get x,y,z values individually
        double x() const { return e[0]; } // inline method definition for short methods
        double y() const { return e[1]; }
        double z() const { return e[2]; }

        // define operator overloads
        // these are member function as they operate on a specific vector instance

        // negate vector - return copy
        vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); } 

        // read-only of array index e.g. vector - this is return-by-value so it returns a COPY
        double operator[](int i) const { return e[i]; }

        // write access to change the vector element - this is returns a reference (double&) so return the actual vector
        double& operator[](int i) { return e[i]; }

        // add another vector to this vector using +=
        vec3& operator+=(const vec3& v){ // pass in by reference (doesnt copy the added vector but const to dont change it)
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            return *this; // return the vector itself (this is a special class pointer, *this dereferences)
        }
        
        // multiply by scalar using *=
        vec3& operator*=(double t){
            e[0] *= t;
            e[1] *= t;
            e[2] *= t;
            return *this;
        }
        
        // alias for element division by scalar (=multiply by 1/t)
        vec3& operator/=(double t){
            return *this *= 1/t; // can use the already defined overloaded operator
        }

        // length information of vector - this is a method so computed only when needed
        double length() const{
            return std::sqrt(length_squared()); // use squared attr
        }

        // length squard
        double length_squared() const{
            return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
        }

        // check if vector near zero to avoid issues
        bool near_zero() const{
            // return true if vector near zero
            auto s = 1e-8;
            // check absolute value of each element using std::fabs
            return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < s);
        }

        // functions to create a random vectors (not unit)
        static vec3 random() {
            // random 0-1 
            return vec3(random_double(), random_double(), random_double());
        }

        static vec3 random(double min, double max) {
            // if a range of values is provided
            return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
        }

}; // semi-colon at end of class definition

// define an alias for vec3 class called point3 -> makes it clearer naming for 3D point vectors
using point3 = vec3;

// define vector utility functions - these define function with vectors as inputs (not member methods)
// use 'inline' to avoid liner error with duplicates

// printing a vector - overload << operator with an output stream
inline std::ostream& operator<<(std::ostream& out, const vec3& v){
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

// adding 2 vectors (return a copy, not a reference so no & here)
inline vec3 operator+(const vec3& u, const vec3& v){
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

// subtracting 2 vectors
inline vec3 operator-(const vec3& u, const vec3& v){
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

// element-wise multiplication
inline vec3 operator*(const vec3& u, const vec3& v){
    return vec3(u.e[0]*v.e[0], u.e[1]*v.e[1], u.e[2]*v.e[2]);
}

// multiply vector by scalar (both t*v and v*t) - overload the * operator for these
inline vec3 operator*(double t, const vec3& v){
    return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline vec3 operator*(const vec3& v, double t){
    return t * v; // pre-multiplying with scalar is equivalent to post-mult
}

// divide by scalar
inline vec3 operator/(const vec3& v, double t){
    return (1/t) * v;
}

// dot product between 2 vectors
inline double dot(const vec3& u, const vec3& v){
    return u.e[0]*v.e[0] + u.e[1]*v.e[1] + u.e[2]*v.e[2];
}

// cross product
inline vec3 cross(const vec3& u, const vec3& v){
    return vec3(
        u.e[1]*v.e[2] - u.e[2]*v.e[1],
        u.e[2]*v.e[0] - u.e[0]*v.e[2],
        u.e[0]*v.e[1] - u.e[1]*v.e[0]
    );
}

// get a unit vector from vector
inline vec3 unit_vector(const vec3& v){
    return v / v.length();
}   

// generate a random unit vector on unit sphere
inline vec3 random_unit_vector() {
    while (true) {
        // loop until valid unit vector found
        auto p = vec3::random(-1, 1); // generate random vector in -1, 1 range (generate on unit cube)
        auto lensq = p.length_squared(); // use squared lenght to constrain to unit sphere
        if (lensq <= 1.0 && lensq > 1e-160) { // handle floating precision errors when create unit vector
            // if length squared within unit sphere, return unit vector
            return p / sqrt(lensq);
        }  
    }
}

// ensure that vector is on the correct hemisphere, by dotting with surface normal
inline vec3 random_on_hemisphere(const vec3& normal) {
    vec3 on_unit_sphere = random_unit_vector(); // generate random unit vector on unit sphere

    // check if in same direction as surface normal vector
    if (dot(on_unit_sphere, normal) > 0.0) {
        return on_unit_sphere;
    }
    else {
        // if not, negate vector and return 
        return -on_unit_sphere;
    }
}

// get reflected ray
inline vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2*dot(v, n)*n; // reflection can be computed by scaling normal vector and subtracting
}

// refract according to snells law
inline vec3 refract(const vec3& v, const vec3& n, double relative_index) {
    auto cos_theta = std::fmin(dot(v, -n), 1.0); // compute cost theta with clipping

    // compute the perpendicular and parallel components of refracted ray
    vec3 r_perp = relative_index * (v + cos_theta*n);
    vec3 r_paral = -std::sqrt(std::fabs(1.0 - r_perp.length_squared())) * n;

    return r_perp + r_paral; // return refracted ray
}

// remember to put endif at the end of the guarded section
#endif 