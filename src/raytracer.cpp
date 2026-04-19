// raytracer.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Each chapter will be tracked in git commits for simplicity.

#include "rtweekend.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

int main() {
	// -- set up objects in the world
	hittable_list world;

	// intialise materials of objects with albedo (and fuzz)
	auto material_ground = make_shared<lambertian>(colour(0.8, 0.8, 0.0));
	auto material_centre = make_shared<lambertian>(colour(0.1, 0.2, 0.5));
	auto material_left   = make_shared<metal>(colour(0.8, 0.8, 0.8), 0.3);
    auto material_right  = make_shared<metal>(colour(0.8, 0.6, 0.2), 1.0);

	// create objects in scene
	world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.2),   0.5, material_centre));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

	// set up camera
	camera cam;

	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 400;
	cam.samples_per_pixel = 500;
	cam.max_depth = 50;

	cam.render(world);
}

