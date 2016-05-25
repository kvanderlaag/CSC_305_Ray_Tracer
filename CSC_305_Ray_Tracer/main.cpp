#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <iostream>
#include "stb_image_write.h"

#include "Vector3.h"
#include "Ray.h"
#include "RaycastObject.h"
#include "RaycastObjectList.h"
#include "Sphere.h"
#include "Plane.h"
#include "Camera.h"
#include "Triangle.h"
#include "Lambertian.h"
#include "Material.h"
#include "Metal.h"
#include "Dielectric.h"
#include "Utility.h"
#include "Cube.h"
#include "Light.h"

#include <ctime>
#include <cstdint>
#include <thread>
#include <future>
#include <random>
#include <cstdlib>
#include <ctime>
#include <cfloat>

#include <ctime>
#include <sstream>

#include "config.h"

std::random_device rd;
std::mt19937 engine(rd());

std::uniform_real_distribution<double> dist;


// Structure for storing 8-bit pixel values. This aligns nicely so that one can
// just allocate an array of them and dump it out to an image using stb_image_write.
struct pixel {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};

/* random_scene()

	Generates a RaycastObjectList composed of a cube of random color, a metal sphere of random
	reflectiveness and color, and a glass sphere of a slightly randomized refractive index, as well
	as a pile of random spheres which (other than the cube) shouldn't overlap.

*/
RaycastObject* random_scene() {
	int n = 500;
	RaycastObject **list = new RaycastObject*[n + 1];

	Vector3 rearBottomLeft(-7.f, 0.f, -7.f);
	Vector3 rearBottomRight(7.f, 0.f, -7.f);
	Vector3 frontBottomLeft(-7.f, 0.f, 7.f);
	Vector3 frontBottomRight(7.f, 0.f, 7.f);

	Vector3 rearTopLeft(-7.f, 14.f, -7.f);
	Vector3 rearTopRight(7.f, 14.f, -7.f);
	Vector3 frontTopLeft(-7.f, 14.f, 7.f);
	Vector3 frontTopRight(7.f, 14.f, 7.f);

	Vector3 topColor(dist(engine), dist(engine), dist(engine));
	Vector3 leftColor(dist(engine), dist(engine), dist(engine));
	Vector3 rightColor(dist(engine), dist(engine), dist(engine));
	Vector3 frontColor(dist(engine), dist(engine), dist(engine));
	Vector3 backColor(dist(engine), dist(engine), dist(engine));
	Vector3 bottomColor(0.5, 0.5, 0.5);

	// Bottom
	list[0] = new Triangle(rearBottomLeft, frontBottomRight, rearBottomRight, new Lambertian(bottomColor));
	list[1] = new Triangle(frontBottomRight, rearBottomLeft, frontBottomLeft, new Lambertian(bottomColor));

	// Back
	list[2] = new Triangle(rearBottomLeft, rearBottomRight, rearTopLeft, new Lambertian(backColor));
	list[3] = new Triangle(rearBottomRight, rearTopRight, rearTopLeft, new Lambertian(backColor));

	// Left
	list[4] = new Triangle(frontBottomLeft, rearBottomLeft, rearTopLeft, new Lambertian(leftColor));
	list[5] = new Triangle(frontBottomLeft, rearTopLeft, frontTopLeft, new Lambertian(leftColor));

	// Right
	list[6] = new Triangle(rearBottomRight, frontBottomRight, rearTopRight, new Lambertian(rightColor));
	list[7] = new Triangle(frontBottomRight, frontTopRight, rearTopRight, new Lambertian(rightColor));

	// Front
	list[8] = new Triangle(frontBottomLeft, frontTopRight, frontBottomRight, new Lambertian(frontColor));
	list[9] = new Triangle(frontTopLeft, frontTopRight, frontBottomLeft, new Lambertian(frontColor));

	// Top
	list[10] = new Triangle(rearTopLeft, rearTopRight, frontTopRight, new Lambertian(topColor));
	list[11] = new Triangle(frontTopRight, frontTopLeft, rearTopLeft, new Lambertian(topColor));

	list[12] = new Cube(Vector3(0, 1, 0), 1.0, new Lambertian(Vector3(dist(engine), dist(engine), dist(engine))));
	list[13] = new Sphere(Vector3(-2, 1, 0), 1.0, new Dielectric(2));
	list[14] = new Sphere(Vector3(2, 1, 0), 1.0, new Metal(Vector3(dist(engine), dist(engine), dist(engine)), 0.2 * dist(engine), dist(engine)));

	
	int i = 15;
	
	
	for (int a = -6; a < 6; a++) {
		for (int b = -6; b < 6; b++) {
			double choose_mat = dist(engine);
			double radius = (dist(engine) * 0.2) + 0.2;
			Vector3 center(a + 0.9*dist(engine), radius, b + 0.9*dist(engine));
			bool overlap = false;
			for (int sphere = 13; sphere < i; ++sphere) {
				if ((center - ((Sphere*)list[sphere])->position).Length() < ((Sphere*)list[sphere])->radius + radius) {
					overlap = true;
					break;
				}
			}
			if (overlap)
				continue;
			if ((center - Vector3(7, radius, 0)).Length() > 0.9) {
				if (choose_mat < 0.8) {  // diffuse
					list[i++] = new Sphere(center, radius, new Lambertian(Vector3(dist(engine)*dist(engine), dist(engine)*dist(engine), dist(engine)*dist(engine))));
				}
				else if (choose_mat < 0.95) { // metal
					list[i++] = new Sphere(center, radius,
						new Metal(Vector3(0.5*(1 + dist(engine)), 0.5*(1 + dist(engine)), 0.5*(1 + dist(engine))), 0.5*dist(engine)));
				}
				else {  // glass
					list[i++] = new Sphere(center, radius, new Dielectric(1 + (dist(engine) * 0.7)));
				}
			}
		}
	}
	

	return new RaycastObjectList(list, i);
}


/* trace_rays()
	This function is called from each rendering thread to compute the color of each pixel in the image.
	It produces double-precision floating point values for every n-th row of the image, where n is the
	thread ID (from 0 to t, where t is the total number of threads).
*/
void trace_rays(int t_id, RaycastObject* world, PointLight** light, int lights, Camera& camera, double* Imagedouble, double* highest_color, std::mutex* color_mutex, int* pixelcount, int* progress, std::mutex* progress_mutex) {
	// Cast a ray for each pixel in the rows that this thread is responsible for.
	for (int i = t_id; i < height; i += NUM_THREADS) {
		for (int j = 0; j < width; ++j) {

			// Cast a ray through each pixel, for each sample.
			Vector3 col(0, 0, 0);
			for (int sample = 0; sample < SAMPLES; ++sample) {
				double u = double(j + (dist(engine))) / width;
				double v = double(i + (dist(engine))) / height;
				//col += color(camera.GetRay(u, v), world, BOUNCES);
				col += lit_color(camera.GetRay(u, v), world, light, lights, BOUNCES);
			}
			// Divide the total color value by the number of samples taken.
			col /= SAMPLES;

			/* Update the maximum color values seen to this point.
			   Prevent other threads from overwriting this value using color_mutex.
			*/
			color_mutex->lock();
			if (col.x > *highest_color)
				*highest_color = col.x;
			if (col.y > *highest_color)
				*highest_color = col.y;
			if (col.z > *highest_color)
				*highest_color = col.z;
			color_mutex->unlock();

			// Set the color values of the pixel in the array of doubles representing
			// the image.
			Imagedouble[3*((width * height - 1) - ((i + 1) * width) + j + 1)] = col.x;
			Imagedouble[3*((width * height - 1) - ((i + 1) * width) + j + 1) + 1] = col.y;
			Imagedouble[3*((width * height - 1) - ((i + 1) * width) + j + 1) + 2] = col.z;
			
			// Update progress. Prevent other threads from overwriting pixelcount using
			// progress_mutex.
			progress_mutex->lock();
			*pixelcount += 1;
			int tempProgress = (int) (*pixelcount / (double) (width * height) * 100);
			if (tempProgress > *progress) {
				*progress = tempProgress;
				system("cls");
				std::cout << *progress << "%";
			}
			progress_mutex->unlock();
		}
	}
}

/* One method of tonemapping whereby colors are scaled with the highest color encountered being
	treated as 1.0
*/
void tonemap_from_highest(double* image, const double& highest, const int& row, const int& column) {
	if (highest > 1) {
		image[3 * ((row * width) + column)] /= highest;
		image[3 * ((row * width) + column) + 1] /= highest;
		image[3 * ((row * width) + column) + 2] /= highest;
	}
}

/* Vignettes the intensity of a pixel based on its distance from the center
	of the image, sort of like a crappy camera lens.
*/
void vignette(double* image, const int& row, const int& column) {
	Vector3 center(width / 2, height / 2, 0);
	Vector3 point(column, row, 0);

	double radius = vignetteradius * center.Length();
	double distance = fabs((point - center).Length() / radius);
	distance *= dvignette;
	distance = (1.0 - distance);

	//std::cout << distance << std::endl;

	image[3 * (row * width + column)] *= distance;
	image[3 * (row * width + column) + 1] *= distance;
	image[3 * (row * width + column) + 2] *= distance;
}


/* Another method of tonemapping whereby if a pixel has a color component exceeding 1.0, all
	components of that color are scaled by the highest value encountered in that pixel.
*/
void tonemap_pixel_highest(double* image, const int& row, const int& column) {
	double highest_so_far = 0;
	if (image[3 * ((row * width) + column)] > highest_so_far)
		highest_so_far = image[3 * ((row * width) + column)];
	if (image[3 * ((row * width) + column) + 1] > highest_so_far)
		highest_so_far = image[3 * ((row * width) + column) + 1];
	if (image[3 * ((row * width) + column) + 2] > highest_so_far)
		highest_so_far = image[3 * ((row * width) + column) + 2];

	if (highest_so_far > 1) {
		image[3 * ((row * width) + column)] /= highest_so_far;
		image[3 * ((row * width) + column) + 1] /= highest_so_far;
		image[3 * ((row * width) + column) + 2] /= highest_so_far;
	}
}

/* A very rudimentary tonemap which simply clamps any R, G, or B value between 0 and 1 */
void tonemap_clamp(double* image, const int& row, const int& column) {
	image[3 * ((row * width) + column)] = std::fmin(image[3 * ((row * width) + column)], 1.f);
	image[3 * ((row * width) + column) + 1] = std::fmin(image[3 * ((row * width) + column) + 1], 1.f);;
	image[3 * ((row * width) + column) + 2] = std::fmin(image[3 * ((row * width) + column) + 2], 1.f);;
}

/* Program main function.
	Maybe add an optional argument for output filename, and/or dimensions?
*/
int main(int argc, char* argv[]) {
	
	// Seed the random number generator with the current time.
	engine.seed(time(0));
	// Initialize the random distribution to generate numbers between 0 and 1.
	dist = std::uniform_real_distribution<double>(0.f, 1.f);

	// Allocate Image
	struct pixel* Image = (pixel*) malloc(sizeof(pixel) * width * height);
	memset(Image, 0, sizeof(pixel) * width * height);
	int stride = (sizeof(pixel)) * width; // Set the stride of each row for stb_image_write

	// Create an array of doubles to store floating-point values for image colors.
	double* Imagedouble = (double*)malloc(sizeof(double) * width * height * 3);
	
	// Generate a random scene.
	RaycastObject* world = random_scene();

	
	PointLight* lights[2];
	int numlights = 2;
	lights[0] = new PointLight(Vector3(-6, 6.5, 6), Vector3(1, 1, 1), 75);
	lights[1] = new PointLight(Vector3(5, 0.1, 6), Vector3(dist(engine), dist(engine), dist(engine)), 15);

	
	double aspect = width / (double)height;
	double focus_dist = (lookat - lookfrom).Length();
	

	Camera camera(lookfrom, lookat, cameraup, fov, aspect, lensRadius, focus_dist);

	std::clock_t start;
	double duration;

	start = std::clock();

	std::mutex color_mutex, progress_mutex;
	double highest_color = 0;
	//double average_color = 0;
	int pixelcount = 0, progress = 0;
	std::thread threads[NUM_THREADS];
	for (int t = 0; t < NUM_THREADS; ++t) {
		threads[t] = std::thread(trace_rays, t, world, lights, numlights, camera, Imagedouble, &highest_color, &color_mutex, &pixelcount, &progress, &progress_mutex);
	}

	for (int t = 0; t < NUM_THREADS; ++t) {
		threads[t].join();
	}

	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

	//std::cout << std::endl << "Duration: " << duration << " seconds." << std::endl;
	//std::cout << "Press enter to continue." << std::endl;
	//std::cin.ignore();

	/* Generate 8-bit RGB values to output to a PNG.
		This operates on each pixel, stored as doubles, and outputs to
		an array of pixel structs.
	*/
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {

			//tonemap_from_highest(Imagedouble, highest_color, i, j);

			vignette(Imagedouble, i, j);

			tonemap_pixel_highest(Imagedouble, i, j);
			//tonemap_clamp(Imagedouble, i, j);


			// Does a gamma correction to the power of 1/g, where g = 2.21
			int ir = (int)(255.99 * pow(Imagedouble[3 * ((i * width) + j)], 1 / 2.21));
			int ig = (int)(255.99 * pow(Imagedouble[3 * ((i * width) + j) + 1], 1 / 2.21));
			int ib = (int)(255.99 * pow(Imagedouble[3 * ((i * width) + j) + 2], 1 / 2.21));
			int ia = 255;
			Image[(i * width) + j].r = ir;
			Image[(i * width) + j].g = ig;
			Image[(i * width) + j].b = ib;
			Image[(i * width) + j].a = ia;

		}
	}

	// Create strings for the .png and .hdr output filenames
	std::ostringstream date_png;
	std::ostringstream date_hdr;

	date_png << "Image-";

		
	time_t t = time(0);   // get time now
	struct tm * now = localtime(&t);
	date_png << (now->tm_year + 1900) << '-'
		<< (now->tm_mon + 1) << '-'
		<< now->tm_mday << '-'
		<< now->tm_hour << '-'
		<< now->tm_min << '-'
		<< now->tm_sec;


	date_hdr << date_png.str();
		

	date_png << t;

	date_png << ".png";
	date_hdr << ".hdr";

	// Write out .png and .hdr images.
	stbi_write_png(date_png.str().c_str(), width, height, 4, (void*)Image, stride);
	stbi_write_hdr(date_hdr.str().c_str(), width, height, 3, Imagedouble);

	// Open the PNG image.
	system(date_png.str().c_str());
	return 0;
}