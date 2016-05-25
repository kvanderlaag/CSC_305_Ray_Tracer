#pragma once
#include "RaycastObject.h"
class Plane :
	public RaycastObject
{
public:
	Plane() {};
	Plane(Vector3 p, Vector3 n, Material* m) : point(p), normal(n), mat(m) {};
	~Plane() {};

	virtual bool Hit(const Ray& r, double t_min, double t_max, hit_record& hit) const;

	Vector3 point;
	Vector3 normal;
	Material* mat;
};

