#include "Utility.h"
#include "Light.h"

#include <random>
#include <iostream>

extern std::mt19937 engine;

extern std::uniform_real_distribution<double> dist;

Vector3 random_in_unit_sphere() {
	Vector3 p(1, 1, 1);
	while (p.SquaredLength() > 1.0f) {
		p = 2.0f * Vector3(dist(engine), dist(engine), dist(engine)) - Vector3(1, 1, 1);
	}
	return p;
}

Vector3 random_in_unit_disk() {
	Vector3 p;
	do {
		p = 2 * Vector3(dist(engine), dist(engine), 0) - Vector3(1, 1, 0);
	} while (p.Dot(p) >= 1.0);
	return p;
}

Vector3 reflect(const Vector3& v_in, const Vector3& normal) {
	return v_in - (2 * v_in.Dot(normal) * normal);
}

Vector3 lit_color(const Ray& r, RaycastObject* world, PointLight** light, int lights, int bounces) {
	if (!bounces)
		return Vector3();
	hit_record rec;
	if (world->Hit(r, 0.001f, FLT_MAX, rec)) {
		Vector3 col;
		for (int l = 0; l < lights; ++l) {
			Ray scatter;
			Ray refract;
			Vector3 attenuation;
			Vector3 diffuse, reflected, refracted;
			hit_record shadow;
			shadow.t = FLT_MAX;
			Ray shadowray(rec.point, (light[l]->position - rec.point).Normalized());
			double reflection = 0;
			if (bounces > 0 && rec.mat->Scatter(r, rec, attenuation, reflection, scatter, refract) && reflection > 0) {
				if (refract.direction.SquaredLength() > 0) {
					refracted = 1.5 * (1.0 - reflection) * lit_color(refract, world, &light[l], 1, bounces - 1);
					reflected = 0.5 * reflection * shadowray.direction.Normalized().Dot(-r.direction.Normalized())  * lit_color(scatter, world, &light[l], 1, (bounces - 1) / 2);
				}
				else {
					reflected = reflection * shadowray.direction.Normalized().Dot(-r.direction.Normalized())  * lit_color(scatter, world, &light[l], 1, bounces - 1);
				}
			}
			
			if (!world->Hit(shadowray, 0.001f, (light[l]->position - rec.point).Length(), shadow)) {
				double ratio = rec.normal.Normalized().Dot(shadowray.direction.Normalized());
				diffuse = attenuation * ratio * (light[l]->color) * (light[l]->intensity / (light[l]->position - rec.point).SquaredLength());
			}
			
			col += reflected + refracted + diffuse;
		}
		return col;
	}

	return Vector3(0, 0, 0);
}

Vector3 color(const Ray& r, RaycastObject* world, int bounces) {
	hit_record rec;
	if (world->Hit(r, 0.001f, FLT_MAX, rec)) {
		Ray scatter;
		Vector3 attenuation;
		double reflection;
		if (bounces > 0 && rec.mat->Scatter(r, rec, attenuation, reflection, scatter, Ray())) {
			return attenuation*color(scatter, world, bounces - 1);
		}
		else {
			return Vector3(0, 0, 0);
		}
	}
	else {
		Vector3 unit_direction = r.direction.Normalized();
		double t = 0.5*(unit_direction.y + 1.0);
		return (1.0 - t)*Vector3(1.0, 1.0, 1.0) + t*Vector3(0.5, 0.7, 1.0);
	}
}



bool refract(const Vector3& v, const Vector3& n, double ni_over_nt, Vector3& refracted) {
	Vector3 uv = v.Normalized();
	double dt = uv.Dot(n);
	double discriminant = 1.0f - ni_over_nt*ni_over_nt*(1.0f - dt*dt);
	if (discriminant > 0) {
		refracted = ni_over_nt*(uv - n*dt) - n*sqrt(discriminant);
		return true;
	}
	else {
		return false;
	}
}

double schlick(double cosine, double ref_idx) {
	double r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return r0 + (1 - r0)*pow(1 - cosine, 5);
}