#pragma once

#include "Vector3.h"
#include "Ray.h"
#include "RaycastObject.h"

class RaycastObject;
class Ray;
class Vector3;

struct hit_record;

class Material
{
public:

	virtual ~Material() {}

	virtual bool Scatter(const Ray& r_in, const hit_record& rec, Vector3& attenuation, double& reflection, Ray& scatter, Ray& refracted) const = 0;
};

