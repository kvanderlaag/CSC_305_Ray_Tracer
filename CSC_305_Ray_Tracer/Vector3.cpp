#include "Vector3.h"
#include <cmath>

Vector3::Vector3() :
	x(0),
	y(0),
	z(0)
{
}

Vector3::Vector3(double x, double y, double z) : 
	x(x),
	y(y),
	z(z)
{

}

Vector3::Vector3(const Vector3& other) :
	x(other.x),
	y(other.y),
	z(other.z)
{

}

Vector3& Vector3::operator=(const Vector3& other) {
	x = other.x;
	y = other.y;
	z = other.z;
	return *this;
}

double Vector3::Length() const {
	return sqrt(x * x + y * y + z * z);
}

double Vector3::SquaredLength() const {
	return x * x + y * y + z * z;
}

Vector3& Vector3::Normalize() {
	double k = 1.0f / sqrt(x * x + y * y + z * z);
	x *= k;
	y *= k;
	z *= k;
	return *this;
}

Vector3& Vector3::operator+=(const Vector3& other) {
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

Vector3& Vector3::operator-=(const Vector3& other) {
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}

Vector3& Vector3::operator*=(const Vector3& other) {
	x *= other.x;
	y *= other.y;
	z *= other.z;
	return *this;
}

Vector3& Vector3::operator/=(const Vector3& other) {
	x /= other.x;
	y /= other.y;
	z /= other.z;
	return *this;
}

Vector3& Vector3::operator*=(const double other) {
	x *= other;
	y *= other;
	z *= other;
	return *this;
}

Vector3& Vector3::operator/=(const double other) {
	x /= other;
	y /= other;
	z /= other;
	return *this;
}

double Vector3::Dot(const Vector3& other) const {
	return (x * other.x) + (y * other.y) + (z * other.z);
}

Vector3 Vector3::Cross(const Vector3& other) const {
	return Vector3((y * other.z - z * other.y),
		(z * other.x) - (x * other.z),
		(x * other.y - y * other.x));
}

Vector3 Vector3::Normalized() const {
	double k = 1.0f / sqrt(x * x + y * y + z * z);
	return Vector3(x * k, y * k, z * k);
}