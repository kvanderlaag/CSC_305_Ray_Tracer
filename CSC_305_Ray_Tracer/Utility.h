#pragma once

#include "Vector3.h"
#include "Ray.h"
#include "Material.h"
#include "RaycastObject.h"
#include "Light.h"

/*
	utility.h

	Defines some function signatures for useful utility functions like reflection, refraction,
	and color computation.
*/

class Vector3;
class Ray;
class Material;
class RaycastObject;

Vector3 reflect(const Vector3& v_in, const Vector3& normal);
Vector3 random_in_unit_sphere();
Vector3 random_in_unit_disk();
Vector3 color(const Ray& r, RaycastObject* world, int bounces);
Vector3 lit_color(const Ray& r, RaycastObject* world, PointLight** light, int lights, int bounces);
bool refract(const Vector3& v, const Vector3& n, double ni_over_nt, Vector3& refracted);

double schlick(double cosine, double ref_idx);