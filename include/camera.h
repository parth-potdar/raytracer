#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h" // include hittable class for rendering
#include "material.h" // include materials

class camera {
    public:
        /* Public camera parameters and render() method*/

        // public parameters (defaults)
        double aspect_ratio = 1.0; // ratio of image width over height
	    int image_width = 100; // rendered image width in pixel count
        int samples_per_pixel = 10; // number of random samples used in anti-aliasing for each pixel 
        int max_depth = 10; // maximum recursion depth of ray bounces in scene (see ray_colour())

        double vfov = 90; // Vertical view angle in degrees (field of view)
        point3 lookfrom = point3(0,0,0); // camera's position in scene
        point3 lookat = point3(0,0,-1); // what coordinate is the camera looking at
        vec3 vup = vec3(0,1,0); // camera-relative up direction -> can rotate this to change viewing angles

        double defocus_angle = 0; // cone angle for defocus disk -> simlulates camera lens
        double focus_dist = 10; // distance from camera and focus plane (perfect focus plane) -> depth of field

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
                   colour pixel_colour(0,0,0);
                   // take multiple samples
                   for (int sample = 0; sample < samples_per_pixel; sample++) {
                    ray r = get_ray(i , j); // get a new ray for each pixel, per sample
                    pixel_colour += ray_colour(r, max_depth, world); // accumulate pixel colour
                   }
                   // colour pixel
                    write_color(std::cout, pixel_samples_scale * pixel_colour);
                }
            }
            // finished message
            std::clog << "\rDone.                           \n";
        }

    private:
        /* Private camera variables and initialisation method*/

        // declare private variables
        int image_height; // rendered image height
        double pixel_samples_scale; // colour scaling factor for a sum of samples
        point3 centre; // camera centre
        point3 pixel00_loc; // location of pixel 0,0
        vec3 pixel_delta_u; // offset to pixel to right
        vec3 pixel_delta_v; // offset to pixel below

        vec3 u, v, w; // orthonormal basis vector for camera frame

        vec3 defocus_u; // defocus disk: horizontal radius
        vec3 defocus_v; // defocus disk: vertical radius

        void initialise() {
            /* Intialise the calculated private camera parameters */

            // calculate image height, ensuring valid values
            image_height = int(image_width / aspect_ratio); // truncates
            image_height = (image_height < 1) ? 1 : image_height;

            // compute pixel_samples_scale = averaging fraction
            pixel_samples_scale = 1.0 / samples_per_pixel;

            centre = lookfrom;

            // -- set up camera and viewport            
            // compute viewport height from fov
            auto theta = degrees_to_radians(vfov); // vertical fov angle in radians
            auto h = std::tan(theta/2.0); // from triangle with z axis (relative to focal length)
            auto viewport_height = 2 * (h * focus_dist); // actual viewport height is 2 times h*f

            auto viewport_width = viewport_height * (double(image_width)/image_height); // compute width from actual aspect ratio
             
            // calculate u,v,w basis vectors for camera coordinate frame
            w = unit_vector(lookfrom - lookat); // point away from looking direction
            u = unit_vector(cross(vup, w)); // cross prod with up vector and looking direction (on camera plane)
            v = cross(w,u); // final basis vector

            // calculate viewport plane vectors using basis vectors
            auto viewport_u = viewport_width * u; // right
            auto viewport_v = viewport_height * -v; // down

            // calculate 'stepping vectors' delta_v and delta_u
            // these give you the vector to move along pixel centres in viewport coordinates
            pixel_delta_u = viewport_u / image_width;
            pixel_delta_v = viewport_v / image_height;

            // calculate origin point of viewport plane (top left corner)
            // first bit moves from camera centre to viewport plane (along -Z axis)
            // then move from viewport centre to top left
            auto viewport_upper_left = centre - (focus_dist*w) - viewport_u/2 - viewport_v/2;

            // from this, we inset by half pixel length to get the centre of the top left pixel
            pixel00_loc = viewport_upper_left + 0.5*(pixel_delta_u + pixel_delta_v);

            // calculate the defocus disk basis vectors
            auto defocus_radius = focus_dist *  std::tan(degrees_to_radians(defocus_angle / 2)); // from cone angle
            defocus_u = defocus_radius * u;
            defocus_v = defocus_radius * v;
        }

        colour ray_colour(const ray& r, int depth, const hittable& world) {
            // define the colour of a ray

            // limit depth of recursion with terminating base case
            if (depth <= 0) {
                return colour(0,0,0); // return black
            }

            // use hittables list (world)
            hit_record rec;
            if (world.hit(r, interval(0.001, infinity), rec)) {
               ray scattered; 
               colour attentuation;
               if (rec.mat->scatter(r, rec, attentuation, scattered)){
                    return attentuation * ray_colour(scattered, depth-1, world);
               }
               return colour(0,0,0);
            }

            // linear interpolation on y value of ray direction 
            // using 0 -> 1 parameter 'a' (a=1 is blue and a=0 is white)
            vec3 unit_direction = unit_vector(r.direction());
            auto a = 0.5*(unit_direction.y() + 1.0); 
	        return (1-a)*colour(1.0, 1.0, 1.0) + a*colour(0.5, 0.7, 1.0);
        }

        ray get_ray(int i, int j) const {
            // create a camera ray from defocus disk (on sensor) to a random sample point around a pixel location i,j

            auto offset = sample_square(); // random sample from a unit square

            // sampled point on image plane
            auto pixel_sample = pixel00_loc
                                + ((i + offset.x()) * pixel_delta_u) // go pixel (i*pixel_delta), but also random offset
                                + ((j + offset.y()) * pixel_delta_v);
            
            auto ray_origin = (defocus_angle < 0) ? centre : defocus_disk_sample();
            auto ray_direction = pixel_sample - ray_origin;

            return ray(ray_origin, ray_direction);
        }

        vec3 sample_square() const {
            // sample random vector in the unit square in range: +- 0.5
            return vec3(random_double() - 0.5, random_double() - 0.5, 0);
        }

        vec3 defocus_disk_sample() const {
            // return a random point in the camera defocus disk
            auto p = random_in_unit_disk();
            return centre + (p[0]*defocus_u) + (p[1]*defocus_v);
        }
};

#endif