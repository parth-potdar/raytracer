// raytracer.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Each chapter will be tracked in git commits for simplicity.

#include "rtweekend.h"

// include hittable object headers
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

colour ray_colour(const ray& r, const hittable& world){
	// define the colour of a ray

	// use hittables list (world)
	hit_record rec;
	if (world.hit(r, interval(0, infinity), rec)) { // if hit, colour by normal
		return 0.5 * (rec.normal + colour(1,1,1));
	}

	// linear interpolation on y value of ray direction 
	// using 0 -> 1 parameter 'a' (a=1 is blue and a=0 is white)
	vec3 unit_direction = unit_vector(r.direction());
	auto a = 0.5*(unit_direction.y() + 1.0); //
	return (1-a)*colour(1.0, 1.0, 1.0) + a*colour(0.5, 0.7, 1.0);
}

int main() {
	// -- set up image: fix aspect ratio and width -> calculate height
	auto aspect_ratio = 16.0 / 9.0; // 16:9 aspect ratio of image
	int image_width = 400;

	// calculate image height, ensuring valid values
	int image_height = int(image_width / aspect_ratio); // truncates
	if (image_height < 1){
		image_height = 1; // set to 1 if computed height is less than 1
	}

	// -- set up objects in the world
	hittable_list world;
	world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
	world.add(make_shared<sphere>(point3(0,-100.5,-1), 100)); // ground plane as big sphere

	// -- set up camera and viewport
	auto focal_length = 1.0; // distace between camera centre and viewport
	auto viewport_height = 2.0; // height of viewport plane (-1 -> 1)
	auto viewport_width = viewport_height * (double(image_width)/image_height); // compute width from actual aspect ratio
	auto camera_centre = point3(0,0,0);

	// calculate viewport plane vectors
	auto viewport_u = vec3(viewport_width, 0, 0); // along width ->
	auto viewport_v = vec3(0, -viewport_height, 0); // down the height

	// calculate 'stepping vectors' delta_v and delta_u
	// these give you the vector to move along pixel centres in viewport coordinates
	auto pixel_delta_u = viewport_u / image_width;
	auto pixel_delta_v = viewport_v / image_height;

	// calculate origin point of viewport plane (top left corner)
	// first bit moves from camera centre to viewport plane (along -Z axis)
	// then move from viewport centre to top left
	auto viewport_upper_left = camera_centre - vec3(0,0,focal_length) - viewport_u/2 - viewport_v/2;

	// from this, we inset by half pixel length to get the centre of the top left pixel
	auto pixel00_loc = viewport_upper_left + 0.5*(pixel_delta_u + pixel_delta_v);

	// -- render the image as plaintext in ppm format
	// first create the header information (P3 - colours in ascii, image size, 255 max channel values for RGB)
	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	// create the image pixel-by-pixel (outer loop on rows, inner loop on columns to match ppm format)
	for (int j = 0; j < image_height; j++) {

		// create progress indicator using std::clog -> like cout but for logging
		std::clog << "\rRows remaining: " << (image_height - j) << ' ' << std::flush; // buffered, so must be fushed

		for (int i = 0; i < image_width; i++) {
			// go to next pixel centre
			auto pixel_centre = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);

			// compute ray direction for that pixel
			auto ray_direction = pixel_centre - camera_centre;
			
			// -- create ray from camera through pixel
			ray r(camera_centre, ray_direction);
			
			// use new colouring function
			colour pixel_colour = ray_colour(r, world);

			write_color(std::cout, pixel_colour);
		}
	}
	// finished message
	std::clog << "\rDone.                           \n";
}

