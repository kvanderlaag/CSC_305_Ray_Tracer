#pragma once

#include "Vector3.h"
#include "Ray.h"
#include "Utility.h"

#include <random>

extern std::mt19937 engine;
extern std::uniform_real_distribution<double> dist;

class Dielectric : public Material
{
public:

	Dielectric(double ri) : ref_index(ri) {}

	virtual bool Scatter(const Ray& r_in, const hit_record& rec, Vector3& attenuation, double& reflection, Ray& scatter, Ray& refracted) const {
		Vector3 outward_normal;
		Vector3 reflected1 = reflect(r_in.direction, rec.normal.Normalized());
		double ni_over_nt;
		attenuation = Vector3(0, 0, 0);
		Vector3 refracted1;
		double reflect_prob;
		double cosine;
		if (r_in.direction.Normalized().Dot(rec.normal.Normalized()) > 0) {
			outward_normal = -rec.normal.Normalized();
			ni_over_nt = ref_index;
			cosine = r_in.direction.Normalized().Dot(rec.normal.Normalized()) / r_in.direction.Length();
			cosine = sqrt(1 - ref_index*ref_index*(1 - cosine*cosine));
		}
		else {
			outward_normal = rec.normal.Normalized();
			ni_over_nt = 1.0f / ref_index;
			cosine = -r_in.direction.Normalized().Dot(rec.normal.Normalized()) / r_in.direction.Length();
		}

		if (refract(r_in.direction, outward_normal, ni_over_nt, refracted1)) {
			reflect_prob = schlick(cosine, ref_index);
		}
		else {
			reflect_prob = 1.0f;
		}

		reflection = reflect_prob;
		scatter = Ray(rec.point, reflected1);
		refracted = Ray(rec.point, refracted1);
		
		return true;
	}

	double ref_index;
};

