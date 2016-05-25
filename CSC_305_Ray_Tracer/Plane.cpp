#include "Plane.h"
#include <cmath>

bool Plane::Hit(const Ray& r, double t_min, double t_max, hit_record& hit) const {
	bool hit_plane = false;


	if (r.direction.Dot(normal) > 0) {
		return false;
	}
	// new 
	/*

	double denom = normal.Dot(r.direction);
	if (abs(denom) > 0.0001f) // your favorite epsilon
	{
		double t = (point - r.origin).Dot(normal) / denom;
		if (t >= 0) {
			if (t >= t_min && t <= t_max) {
				hit.normal = normal;
				hit.point = r.origin + t * r.direction;
				hit.t = t;
				hit.mat = mat;
				hit_plane = true;
			}
		}// you might want to allow an epsilon here too
	}
	return hit_plane;
	*/


	double denom = normal.Dot(r.direction);

	if (denom == 0) {
		return false;
	}

	double t = (point - r.origin).Dot(normal) / denom;

	if (t < 0.f) {
		return false;
	}

	if (t >= t_min && t <= t_max) {
		hit.normal = normal;
		hit.point = r.origin + t * r.direction;
		hit.t = t;
		hit.mat = mat;
		hit_plane = true;
	}

	return hit_plane;
}