#pragma once
#ifndef _VECTOR3_H
#define _VECTOR3_H

/* 
	Vector3.h

	Defines class data and function members for a three-dimensional vector.

*/

class Vector3
{
public:
	Vector3();
	Vector3(double x, double y, double z);
	Vector3(const Vector3& other);

	Vector3& operator=(const Vector3& other);
	Vector3& operator+=(const Vector3& other);
	Vector3& operator-=(const Vector3& other);
	Vector3& operator*=(const Vector3& other);
	Vector3& operator/=(const Vector3& other);
	Vector3& operator*=(const double other);
	Vector3& operator/=(const double other);
	bool operator==(const Vector3& other) const {
		return (x == other.x && y == other.y && z == other.z);
	}

	bool operator!=(const Vector3& other) const {
		return !(*this == other);
	}

	Vector3& Normalize();
	double Length() const;
	double SquaredLength() const;

	double Dot(const Vector3& other) const;
	Vector3 Cross(const Vector3& other) const;

	Vector3 Normalized() const;


	double x, y, z;
};

inline Vector3 operator*(const Vector3& lhs, const double rhs) {
	return Vector3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
}

inline Vector3 operator*(double lhs, const Vector3& rhs) {
	return Vector3(rhs.x * lhs, rhs.y * lhs, rhs.z * lhs);
}

inline Vector3 operator/(const Vector3& lhs, double rhs) {
	return Vector3(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
}

inline Vector3 operator/(const Vector3& lhs, int rhs) {
	return Vector3(lhs.x / (double) rhs, lhs.y / (double) rhs, lhs.z / (double) rhs);
}

inline Vector3 operator+(const Vector3& lhs, const Vector3& rhs) {
	return Vector3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

inline Vector3 operator-(const Vector3& lhs, const Vector3& rhs) {
	return Vector3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

inline Vector3 operator/(const Vector3& lhs, const Vector3& rhs) {
	return Vector3(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
}

inline Vector3 operator*(const Vector3& lhs, const Vector3& rhs) {
	return Vector3(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
}

inline Vector3 Lerp(const Vector3& v1, const Vector3& v2, double t) {
	return v1 + ((v2 - v1) * t);
}

inline Vector3 operator-(const Vector3 v) {
	return Vector3(-v.x, -v.y, -v.z);
}

inline Vector3 operator+(const Vector3 v) {
	return Vector3(v.x, v.y, v.z);
}

#endif