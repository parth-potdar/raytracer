#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <vector> // for vector class -> basically equivalent to Python list

class hittable_list : public hittable {
    public:
        // vector of shared pointers -> hittable objects
        std::vector<shared_ptr<hittable>> objects;

        // Constructors
        hittable_list() {}
        // if pointer to hittable object given, use add method to add to the list
        hittable_list(shared_ptr<hittable> object) { add(object); }

        // clear objects in list using std::vector clear()
        void clear() { objects.clear(); }

        // add objects to the end of the list using std::vector pushback()
        void add(shared_ptr<hittable> object) {
            objects.push_back(object);
        }

        // Check if anything in the list is hit by the ray
        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            // temp record to update in loop
            hit_record temp_rec;

            // intialise boolean output to false
            bool hit_anything = false;
            // initialise closest point so far 
            auto closest_so_far = ray_t.max;

            // loop over the hittable objects in list 
            for (const auto& object : objects) {
                // check if a hit happens and is closer than the closest_so_far
                if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec)){
                    hit_anything = true;
                    closest_so_far = temp_rec.t; // update closest so far
                    rec = temp_rec; //update actual record
                }
            }

            // return whether anything was hit
            return hit_anything;
        }
};
#endif 