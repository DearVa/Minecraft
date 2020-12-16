#include "vector3.h"

inline double Vector3::length() {
	return (float)(x * x + y * y + z * z);
}

Vector3 Vector3::normalize() {
	float len = length();
	if (len == 0) {
		len = 1;
	}
	x = x / len;
	y = y / len;
	z = z / len;

	return *this;
}

double Vector3::dotProduct(const Vector3 &v) {
	return (x * v.x + y * v.y + z * v.z);
}

Vector3 Vector3::crossProduct(const Vector3 &v) {
	Vector3 vec;

	vec.x = y * v.z - z * v.y;
	vec.y = z * v.x - x * v.z;
	vec.z = x * v.y - y * v.x;

	return vec;
}

Vector3 Vector3::operator + (const Vector3 &v) {
	Vector3 vec;

	vec.x = x + v.x;
	vec.y = y + v.y;
	vec.z = z + v.z;

	return vec;
}

Vector3 &Vector3::operator+=(const Vector3 &v) {
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vector3 Vector3::operator - (const Vector3 &v) {
	Vector3 vec;

	vec.x = x - v.x;
	vec.y = y - v.y;
	vec.z = z - v.z;

	return vec;
}

Vector3 &Vector3::operator-=(const Vector3 &v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Vector3 Vector3::operator * (const double scale) {
	Vector3 v;

	v.x = x * scale;
	v.y = y * scale;
	v.z = z * scale;

	return v;
}

Vector3 Vector3::operator / (const double scale) {
	if (scale != 0.0) {
		x = x / scale;
		y = y / scale;
		z = z / scale;
	}
	return *this;
}

Vector3 Vector3::operator - () {
	Vector3 vec(-x, -y, -z);
	return vec;
}

bool Vector3::operator == (const Vector3 &v) {
	return (x == v.x) && (y == v.y) && (z = v.z);
}

Vector3i Vector3i::operator + (const Vector3i &v) {
	Vector3i vec;

	vec.x = x + v.x;
	vec.y = y + v.y;
	vec.z = z + v.z;

	return vec;
};

Vector3i Vector3i::operator + (const Vector3 &v) {
	Vector3i vec;

	vec.x = x + v.x;
	vec.y = y + v.y;
	vec.z = z + v.z;

	return vec;
};

Vector3i Vector3i::operator - (const Vector3i &v) {
	Vector3i vec;

	vec.x = x - v.x;
	vec.y = y - v.y;
	vec.z = z - v.z;

	return vec;
};

Vector3i Vector3i::operator - (const Vector3 &v) {
	Vector3i vec;

	vec.x = x + v.x;
	vec.y = y + v.y;
	vec.z = z + v.z;

	return vec;
};

bool Vector3i::operator == (const Vector3i &v) {
	return (x == v.x) && (y == v.y) && (z = v.z);
}

Vector3i::operator Vector3() {
	return Vector3(x, y, z);
}