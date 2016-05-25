#pragma once
#include "RaycastObject.h"
#include "Plane.h"
#include "Lambertian.h"
class Triangle :
	public RaycastObject
{
public:
	Triangle(Vector3 vv1, Vector3 vv2, Vector3 vv3, Material* m) : v1(vv1), v2(vv2), v3(vv3), normal((vv2 - vv1).Cross(vv3 - vv1)), p(new Plane(vv1, (vv2 - vv1).Cross(vv3 - vv1), new Lambertian(Vector3(0, 0,0)))), mat(m) {};

	virtual bool Hit(const Ray& r, double t_min, double t_max, hit_record& hit) const;

	Vector3 Normal() const;

	void Barycentric(const Vector3& p, double& u, double& v, double& w) const;

	Vector3 v1;
	Vector3 v2;
	Vector3 v3;
	Material* mat;
	Vector3 normal;
	Plane* p;
};

