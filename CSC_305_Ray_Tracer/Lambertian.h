#pragma once
#include "Material.h"
#include "Utility.h"
#include <cmath>

class Lambertian :
	public Material
{
public:
	Lambertian(const Vector3& a) : albedo(a) {}

	// Take a ray and a hit record, generate a color attenuation, reflected ray, and refracted ray.
	virtual bool Scatter(const Ray& r_in, const hit_record& rec, Vector3& attenuation, double& reflection, Ray& scatter, Ray& refracted) const {
		Vector3 target = rec.point + rec.normal.Normalized() + random_in_unit_sphere();
		scatter = Ray(rec.point, target - rec.point);
		refracted = Ray();
		attenuation = albedo;
		reflection = 0.0;
		return true;
	}

	Vector3 albedo;
};

