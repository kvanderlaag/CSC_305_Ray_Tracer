#pragma once
#include "Material.h"
#include "Utility.h"

class Metal :
	public Material
{
public:
	Metal(const Vector3& a, const double& f) : albedo(a), reflectiveness(1.f) { if (f < 1) fuzziness = f; else fuzziness = 0; };
	Metal(const Vector3& a, const double& f, const double& r) : albedo(a), reflectiveness(r) { if (f < 1) fuzziness = f; else fuzziness = 0; };
	virtual bool Scatter(const Ray& r_in, const hit_record& rec, Vector3& attenuation, double& reflection, Ray& scatter, Ray& refracted) const {
		Vector3 reflected1 = reflect(r_in.direction, rec.normal.Normalized());
		scatter = Ray(rec.point, reflected1 + fuzziness * random_in_unit_sphere());
		refracted = Ray(Vector3(0, 0, 0), Vector3(0, 0, 0));
		attenuation = albedo * (1.0f - reflectiveness);
		reflection = reflectiveness;
		return scatter.direction.Dot(rec.normal) > 0;
	}

	Vector3 albedo;
	double fuzziness;
	double reflectiveness;
};