#pragma once
#include <cmath>
#include "vector3.h"

class Vector2 {
public:
	float x, y;

	Vector2() {
		x = 0.0f;
		y = 0.0f;
	}

	Vector2(float x, float y) {
		this->x = x;
		this->y = y;
	}

	Vector2(const Vector2 &v) {
		x = v.x;
		y = v.y;
	}

	Vector2(const Vector3 &v) {
		x = v.x;
		y = v.y;
	}

	inline float length();
	Vector2 normalize();
	float dotProduct(const Vector2 &v);
	Vector2 crossProduct(const Vector2 &v);

	Vector2 operator + (const Vector2 &v);
	Vector2 operator - (const Vector2 &v);
	Vector2 operator * (float scale);
	Vector2 operator / (float scale);
	Vector2 operator - ();
};