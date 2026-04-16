#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h" // include hittable class for rendering

class camera {
    public:
        /* Public camera parameters and render() method*/

        // public parameters (defaults)
        double aspect_ratio = 1.0; // ratio of image width over height
	    int image_width = 100; // rendered image width in pixel count

        void render(const hittable& world) {
            // render the image to cout
            initialise(); // initialise camera given user-defined camera parameters and defaults
            
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
                    auto ray_direction = pixel_centre - centre;
                    
                    // -- create ray from camera through pixel
                    ray r(centre, ray_direction);
                    
                    // use new colouring function
                    colour pixel_colour = ray_colour(r, world);

                    write_color(std::cout, pixel_colour);
                }
            }
            // finished message
            std::clog << "\rDone.                           \n";
        }

    private:
        /* Private camera variables and initialisation method*/

        // declare private variables
        int image_height; // rendered image height
        point3 centre; // camera centre
        point3 pixel00_loc; // location of pixel 0,0
        vec3 pixel_delta_u; // offset to pixel to right
        vec3 pixel_delta_v; // offset to pixel below

        void initialise() {
            /* Intialise the calculated private camera parameters */

            // calculate image height, ensuring valid values
            image_height = int(image_width / aspect_ratio); // truncates
            image_height = (image_height < 1) ? 1 : image_height;

            centre = point3(0,0,0);

            // -- set up camera and viewport
            auto focal_length = 1.0; // distace between camera centre and viewport
            auto viewport_height = 2.0; // height of viewport plane (-1 -> 1)
            auto viewport_width = viewport_height * (double(image_width)/image_height); // compute width from actual aspect ratio
             

            // calculate viewport plane vectors
            auto viewport_u = vec3(viewport_width, 0, 0); // along width ->
            auto viewport_v = vec3(0, -viewport_height, 0); // down the height

            // calculate 'stepping vectors' delta_v and delta_u
            // these give you the vector to move along pixel centres in viewport coordinates
            pixel_delta_u = viewport_u / image_width;
            pixel_delta_v = viewport_v / image_height;

            // calculate origin point of viewport plane (top left corner)
            // first bit moves from camera centre to viewport plane (along -Z axis)
            // then move from viewport centre to top left
            auto viewport_upper_left = centre - vec3(0,0,focal_length) - viewport_u/2 - viewport_v/2;

            // from this, we inset by half pixel length to get the centre of the top left pixel
            pixel00_loc = viewport_upper_left + 0.5*(pixel_delta_u + pixel_delta_v);
        }

        colour ray_colour(const ray& r, const hittable& world) {
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
};

#endif