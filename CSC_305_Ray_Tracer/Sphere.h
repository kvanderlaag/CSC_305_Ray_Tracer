#pragma once
#include "RaycastObject.h"
#include "Material.h"
#include <cmath>

class Sphere :
	public RaycastObject
{
public:
	Sphere(Vector3 p, double r, Material* m) : position(p), radius(r), mat(m) {  };

	~Sphere() {
		delete mat;
	}

	virtual bool Hit(const Ray& r, double t_min, double t_max, hit_record& hit) const;

	Vector3 position;
	double radius;
	Material* mat;
};

