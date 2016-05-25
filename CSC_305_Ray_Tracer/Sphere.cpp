#include "Sphere.h"

bool Sphere::Hit(const Ray& r, double t_min, double t_max, hit_record& hit) const {
	Vector3 oc = r.origin - position;
	double a = r.direction.Dot(r.direction);
	double b = oc.Dot(r.direction);
	double c = oc.Dot(oc) - radius*radius;
	double discriminant = b*b - a*c;
	if (discriminant >= 0) {
		double temp = (-b - sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min) {
			hit.t = temp;
			hit.point = r.PointAt(hit.t);
			hit.normal = (hit.point - position) / radius;
			hit.mat = mat;
			return true;
		}
		temp = (-b + sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min) {
			hit.t = temp;
			hit.point = r.PointAt(hit.t);
			hit.normal = (hit.point - position) / radius;
			hit.mat = mat;
			return true;
		}
	}
	return false;
}