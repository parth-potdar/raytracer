// raytracer.cpp : This file contains the 'main' function. Program execution begins and ends there.

// Each chapter will be tracked in git commits for simplicity.

#include <iostream>

int main() {
	// define the image size
	int image_width = 256;
	int image_height = 256;

	// render the image as plaintext in ppm format
	// first create the header information (P3 - colours in ascii, image size, 255 max channel values for RGB)
	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	// create the image pixel-by-pixel (outer loop on rows, inner loop on columns to match ppm format)
	for (int j = 0; j < image_height; j++) {
		for (int i = 0; i < image_width; i++) {
			// exmaple pixel values
			// 'auto' tells compiler to figure out datatype
			// this is useful when types are verbose or its obvious and it can save time (but less clear to reader sometimes)
			auto r = double(i) / (image_width - 1); 
			auto g = double(j) / (image_height - 1);
			auto b = 0.5;

			// r,g,b values are 0 - 1 and need to be converted to 0 - 255 scale
			// as we cast to integer, there will be truncation. 
			// To get correct truncation (e.g 0.999 -> 1 instead of 0), we multiply by (255 + 0.999) - explicit truncation
			int ir = int(255.999 * r);
			int ig = int(255.999 * g);
			int ib = int(255.999 * b);

			// write pixel values to terminal
			std::cout << ir << ' ' << ig << ' ' << ib << '\n';
		}
	}
}

