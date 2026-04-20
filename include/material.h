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

class dielectric : public material {
    // dielectric material reflects and refracts
    public:
        //constructor
        dielectric(double refraction_index) : refraction_index(refraction_index) {}

        bool scatter(
            const ray& r_in, const hit_record& rec, colour& attentuation, ray& scattered
        ) const override {
            attentuation = colour(1.0, 1.0, 1.0); // zero absorption -> all rays reflect/refract with no loss of colour

            // check if ray is hitting surface or inside -> get correct refraction direction
            // if hitting outside of surface, invert index (bend inwards), otherwise bend outwards
            double ri = rec.front_face ? (1.0/refraction_index) : refraction_index;

            vec3 unit_direction = unit_vector(r_in.direction()); // unit direction vector of incident ray
            
            // handle no-solution to snells law -> Total Interal Reflection
            double cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
            double sin_theta = std::sqrt(1.0 - cos_theta*cos_theta);

            // flag TIR
            bool cannot_refract = ri * sin_theta > 1.0;
            vec3 direction; // declare direction of out ray

            if (cannot_refract || reflectance(cos_theta, ri) > random_double()){ // if TIR -> reflect
                direction = reflect(unit_direction, rec.normal);
            }
            else { // else, always refract
                direction = refract(unit_direction, rec.normal, ri);
            }

            scattered = ray(rec.p, direction);

            return true;
        }
    private:
        double refraction_index; // material refractive index - usually relative to vacuum/air or outer material
        
        static double reflectance(double cosine, double refraction_index) {
            // Schlick's approximation of reflectance of glass
            auto r0 = (1 - refraction_index) / (1 + refraction_index);
            r0 = r0*r0;
            return r0 + (1-r0)*std::pow((1-cosine), 5);
        }
};

#endif