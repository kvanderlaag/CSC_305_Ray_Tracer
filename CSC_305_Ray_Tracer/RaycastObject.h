#pragma once

#include "Ray.h"

class Material;

struct hit_record {
	double t;
	Vector3 point;
	Vector3 normal;
	Vector3 barycentric;
	Material* mat;
};


class RaycastObject
{
public:
	virtual bool Hit(const Ray& r, double t_min, double t_max, hit_record& hit) const = 0;
};

