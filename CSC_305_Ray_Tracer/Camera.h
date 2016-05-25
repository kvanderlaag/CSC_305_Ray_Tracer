#pragma once

#include "Ray.h"
#include "Vector3.h"
#include <cmath>
#include <random>
#include "Utility.h"

extern std::mt19937 engine;
extern std::uniform_real_distribution<double> dist;

class Camera
{
public:
	Camera(Vector3 lookfrom, Vector3 lookat, Vector3 vup, double vfov, double aspect, double aperture, double focus_dist) :
		theta(vfov * ((double)M_PI / 180)),
		aspect(aspect),
		origin(lookfrom)

	{
		lens_radius = aperture / 2;

		//Vector3 u, v, w;
		double half_height = tan(theta / 2);
		double half_width = aspect * half_height;
		
		w = (lookfrom - lookat).Normalized();
		u = vup.Cross(w).Normalized();
		v = w.Cross(u);

		lower_left_corner = origin - half_width*focus_dist*u - half_height*focus_dist*v - focus_dist*w;
		horizontal = 2 * half_width * focus_dist * u;
		vertical = 2 * half_height * focus_dist * v;
	}

	Ray GetRay(double s, double t) { 
		Vector3 rd = lens_radius * random_in_unit_disk();
		Vector3 offset = u * rd.x + v * rd.y;
		return Ray(origin + offset, lower_left_corner + s*horizontal + t*vertical - origin - offset); 
	}

	Vector3 position;
	Vector3 lookAt;
	double theta;
	double aspect;
	Vector3 lower_left_corner;
	Vector3 horizontal;
	Vector3 vertical;
	Vector3 origin;
	Vector3 u, v, w;
	float lens_radius;
};

