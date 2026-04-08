// raytracer.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Each chapter will be tracked in git commits for simplicity.

#include <iostream>

// include the vector and colour header files
#include "vec3.h"
#include "colour.h"

int main() {
	// define the image size
	int image_width = 256;
	int image_height = 256;

	// render the image as plaintext in ppm format
	// first create the header information (P3 - colours in ascii, image size, 255 max channel values for RGB)
	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	// create the image pixel-by-pixel (outer loop on rows, inner loop on columns to match ppm format)
	for (int j = 0; j < image_height; j++) {

		// create progress indicator using std::clog -> like cout but for logging
		std::clog << "\rRows remaining: " << (image_height - j) << ' ' << std::flush; // buffered, so must be fushed

		for (int i = 0; i < image_width; i++) {
			// exmaple pixel values
			// 'auto' tells compiler to figure out datatype
			// this is useful when types are verbose or its obvious and it can save time (but less clear to reader sometimes)
			auto r = double(i) / (image_width - 1); 
			auto g = double(j) / (image_height - 1);
			auto b = 0.5;
			
			auto pixel_colour = colour(r,g,b); // create pixel colour vector
			write_color(std::cout, pixel_colour);
		}
	}
	// finished message
	std::clog << "\rDone.                           \n";
}

