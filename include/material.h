#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

class material {
    // abstract material class
    public:
        // virtual destructor
        virtual ~material() = default;

        // abstract method to overload
        // how does the material scatter/absorb lighy
        virtual bool scatter(
            const ray& r, const hit_record& rec, colour& attentuation, ray& scattered
        ) const {
            // boolean return
            // but we directly modify the input attentuation and scattered vectors (by reference)
            return false;
        }
};

class lambertian : public material {
    // lambertian material
    public:
        //constructor
        lambertian(const colour& albedo) : albedo(albedo) {}

        bool scatter(
            const ray& r_in, const hit_record& rec, colour& attentuation, ray& scattered
        ) const override {
            // simple lambertian scatters with probability 1 and attennuates with reflectance/albedo
            auto scatter_direction = rec.normal + random_unit_vector(); // lambertian distribution

            // handle the case where scatter direction is near 0
            if (scatter_direction.near_zero()){
                scatter_direction = rec.normal;
            }

            scattered = ray(rec.p, scatter_direction); // scattered ray

            attentuation = albedo; // attentuation level

            return true;
        }

    private:
        colour albedo; // albedo describes the 'fractional reflectance' of lamnbertian material
};

class metal : public material {
    // metal class (reflective)
    public:
        //constructor - provide albedo and fuzz radius (secondary sphere radius to 'fuzz' the reflected ray)
        metal(const colour& albedo, double fuzz) : albedo(albedo), fuzz(fuzz) {}

        bool scatter(
            const ray& r_in, const hit_record& rec, colour& attentuation, ray& scattered
        ) const override {
            // reflected ray
            vec3 reflected = reflect(r_in.direction(), rec.normal);

            // apply fuzz to the reflected vector
            reflected = unit_vector(reflected) + (fuzz * random_unit_vector());

            scattered = ray(rec.p, reflected);

            attentuation = albedo;

            // only returned true if scattered (if absorbed e.g. fuzz points inside surface)
            return (dot(scattered.direction(), rec.normal) > 0);
        }

    private:
        colour albedo;
        double fuzz;
};

#endif