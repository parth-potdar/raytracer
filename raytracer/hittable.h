#ifndef HITTABLE_H // header guard
#define HITTABLE_H

class hit_record{
    // this structure stores a hit's point of intersection, normal vector and the value t along the ray
    public:
        point3 p; // P(t)
        vec3 normal; // normal vector
        double t; // coordinate along ray (t)
        bool front_face; // set normal vector always be against the ray direction

        void set_face_normal(const ray& r, const vec3& outward_normal) {
            // set the hit_record normal vector from a computed unit normal vector
            // -> stores if inside or outside

            // check if dot product is negative -> ray and normal in opposite directions
            front_face = dot(r.direction(), outward_normal) < 0;
            // set correct normal direction (if front_face is true, no change - else, negate)
            normal = front_face ? outward_normal: -outward_normal;
        }
};

class hittable{
    // hittable is an 'Abstract Class', so objects can only inherit from this e.g. a sphere
    public:
        // virtual destructor -> means if sphere deleted through hittable* pointer, sphere destructor -> then hittable
        // set behaviour to 'default' - same as leaving empty {} -> this means compiler generates destructor for you
        // it writes the 'code' to destroy member variabels in order (more imporant for the subclass)
        virtual ~hittable() = default; 
        // not specifing a constructor either

        // hit method: must be overridden by subclass and take the following inputs
        virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};

#endif