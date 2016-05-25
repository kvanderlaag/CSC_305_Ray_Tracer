#include "Triangle.h"
#include "Plane.h"
#include "Lambertian.h"

#include <cfloat>
#include <cmath>
#include <cstdio>

/*
	Triangle.cpp

	Defines functionality for computing intersection between a ray and a triangle,
	and barycentric coordinates on that triangle.

*/

void Triangle::Barycentric(const Vector3& p, double &u, double &v, double &w) const
{
	Vector3 vv0 = v2 - v1;
	Vector3 vv1 = v3 - v1;
	Vector3 vv2 = p - v1;
	double d00 = vv0.Dot(vv0);
	double d01 = vv0.Dot(vv1);
	double d11 = vv1.Dot(vv1);
	double d20 = vv2.Dot(vv0);
	double d21 = vv2.Dot(vv1);
	double invDenom = 1.0 / (d00 * d11 - d01 * d01);
	v = (d11 * d20 - d01 * d21) * invDenom;
	w = (d00 * d21 - d01 * d20) * invDenom;
	u = 1.0f - v - w;
}

Vector3 Triangle::Normal() const {
	return normal;
	return (v2 - v1).Cross(v3 - v1);
}

bool Triangle::Hit(const Ray& r, double t_min, double t_max, hit_record& hit) const {
	bool hit_triangle = false;

	if (r.direction.Dot(Normal()) > 0) {
		return hit_triangle;
	}

	hit_record temp_rec;
	if (p->Hit(r, t_min, t_max, temp_rec)) {
		Barycentric(temp_rec.point, temp_rec.barycentric.x, temp_rec.barycentric.y, temp_rec.barycentric.z);
		if (temp_rec.barycentric.x < 0.0f || temp_rec.barycentric.y < 0.0f || temp_rec.barycentric.z < 0.0f) {

		} else {
			hit = temp_rec;
			//hit.point = temp_rec.point;
			hit.normal = normal.Normalized();
			//hit.t = temp_rec.t;
			//hit.barycentric = temp_rec.barycentric;
			hit.mat = mat;
			hit_triangle = true;
		}
	}

	return hit_triangle;
}