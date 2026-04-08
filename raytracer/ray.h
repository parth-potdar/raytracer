// header file for ray class
#ifndef RAY_H
#define RAY_H

#include "vec3.h"

// ray class 
// rays can be described as: P(t) = A + b*t
// where A is the origin of the ray, and b is the direction
class ray{
    public:
        // constructors
        ray() {} // no values -> empty ray

        // if provided, set orig and dir attibutes in member initialiser list (after : below)
        ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {};

        // getters: using 1st const to say caller cannot modify return type
        // 2nd const to say method itself wont modify
        // the const return means getters return immutable reference (instead of a copy)
        const point3& origin() const { return orig; }
        const vec3& direction() const { return dir;}

    private:
        // the rays origin and direction are private attributes
        point3 orig;
        vec3 dir;
};
#endif