// raytracer.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Each chapter will be tracked in git commits for simplicity.

#include "rtweekend.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

int main() {
	// -- set up objects in the world
	hittable_list world;
	world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
	world.add(make_shared<sphere>(point3(0,-100.5,-1), 100)); // ground plane as big sphere

	// set up camera
	camera cam;

	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 400;
	cam.samples_per_pixel = 100;
	cam.max_depth = 50;

	cam.render(world);
}

