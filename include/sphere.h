#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h" // already includes ray

class sphere : public hittable {
    // Sphere subclass (inherits from base class - hittable). 'public' means public methods/attr of base stay public

    public:
        // add a constructor which initialises centre and radius member variables ( : centre(...), ...)
        // use std::fmax to apply minimum radius value (0) - fmax returns maximum of 2 floating point numbers
        sphere(const point3& centre, double radius) : centre(centre), radius(std::fmax(0, radius)) {}

        // define hit function - optional keyword 'override' to tell compiler to catch errors
        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            // logic to find intersection point between ray and sphere (same as before)
            vec3 oc = centre - r.origin();
            auto a = r.direction().length_squared();
            auto h = dot(r.direction(), oc);
            auto c = oc.length_squared() - radius*radius;

            // evaluate discriminant (with b = -2h)
            auto discriminant = h*h - a*c;

            // find intersection points
            if (discriminant < 0) {
                // if no intersection (negative discriminant)
                return false;
            }
            
            // sqrt discirminant
            auto sqrtd = std::sqrt(discriminant);

            // find the nearest root that lies in the acceptable range tmin and tmax
            auto root  = (h - sqrtd) / a;
            
            // if root is not within range, try the other solution
            if (!ray_t.surrounds(root)) {
                root  = (h + sqrtd) / a;
                // if this also isnt within range, return false
                if (!ray_t.surrounds(root)) {
                    return false;
                }
            }
            
            // update hit_record struct values
            rec.t = root;
            rec.p = r.at(rec.t);
            vec3 outward_normal = (rec.p - centre) / radius; // compute outwards normal, and then convert   
            rec.set_face_normal(r, outward_normal); // let hit_record object set the correct normal direction

            // return true if valid intersection found -> object hit
            return true;
        }

    private:
        point3 centre; // declate member variables as private
        double radius;
};

#endif