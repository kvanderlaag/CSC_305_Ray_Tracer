#include "Light.h"

#include <cmath>

bool PointLight::Hit(const Ray& r, double t_min, double t_max, hit_record& hit) const {
	Vector3 oc = r.origin - position;
	double a = r.direction.Dot(r.direction);
	double b = oc.Dot(r.direction);
	double c = oc.Dot(oc);
	double discriminant = b*b - a*c;
	if (discriminant >= 0) {
		double temp = (-b - sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min) {
			hit.t = temp;
			hit.point = r.PointAt(hit.t);
			hit.normal = (hit.point - position);
			return true;
		}
		temp = (-b + sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min) {
			hit.t = temp;
			hit.point = r.PointAt(hit.t);
			hit.normal = (hit.point - position);
			return true;
		}
	}
	return false;
}