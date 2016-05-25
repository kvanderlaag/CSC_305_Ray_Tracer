#pragma once
#include "Vector3.h"

class Vector3;

class Ray
{
public:
	Ray() : origin(Vector3(0, 0, 0)), direction(Vector3(0, 0, 0)) {}
	Ray(const Vector3& Origin, const Vector3& Direction) { origin = Origin; direction = Direction; }

	bool operator==(const Ray& other) const {
		return (origin == other.origin && direction.Normalized() == other.direction.Normalized());
	}

	bool operator!=(const Ray& other) const {
		return !(*this == other);
	}

	inline Vector3 PointAt(double t) const { return origin + t * direction; }

	Vector3 origin;
	Vector3 direction;


};

