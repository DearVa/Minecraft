#include "Vector2.h"

inline float Vector2::length() {
	return (float)(x * x + y * y);
}

Vector2 Vector2::normalize() {
	float len = length();
	if (len == 0) {
		len = 1;
	}
	x = x / len;
	y = y / len;

	return *this;
}

float Vector2::dotProduct(const Vector2 &v) {
	return (x * v.x + y * v.y);
}

Vector2 Vector2::crossProduct(const Vector2 &v) {
	Vector2 vec;

	vec.x = y * v.x - x * v.y;
	vec.y = x * v.y - y * v.x;

	return vec;
}

Vector2 Vector2::operator +(const Vector2 &v) {
	Vector2 vec;

	vec.x = x + v.x;
	vec.y = y + v.y;

	return vec;
}

Vector2 Vector2::operator -(const Vector2 &v) {
	Vector2 vec;

	vec.x = x - v.x;
	vec.y = y - v.y;

	return vec;
}

Vector2 Vector2::operator *(float scale) {
	x = x * scale;
	y = y * scale;

	return *this;
}

Vector2 Vector2::operator /(float scale) {
	if (scale != 0.0) {
		x = x / scale;
		y = y / scale;
	}
	return *this;
}

Vector2 Vector2::operator -() {
	Vector2 vec(-x, -y);
	return vec;
}