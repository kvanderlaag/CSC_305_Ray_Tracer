#pragma once

#include "RaycastObject.h"
#include "Vector3.h"

class PointLight : public RaycastObject
{
public:
	PointLight() : position(Vector3(0, 0, 0)), color(Vector3(1, 1, 1)), intensity(1.0f) {}
	PointLight(Vector3 position, Vector3 color) : position(position), color(color), intensity(1.0f) {}
	PointLight(Vector3 position, Vector3 color, double intensity) : position(position), color(color), intensity(intensity) {}

	bool Hit(const Ray& r, double t_min, double t_max, hit_record& hit) const;

	Vector3 position;
	Vector3 color;
	double intensity;
};

