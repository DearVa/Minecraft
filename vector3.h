#pragma once
#include <cmath>

class Vector3 {
public:
	float x, y, z;

	Vector3() {
		x = 0.0;
		y = 0.0;
		z = 0.0;
	}

	Vector3(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vector3(const Vector3 &v) {
		x = v.x;
		y = v.y;
		z = v.z;
	}

	inline float length();
	Vector3 normalize();
	float dotProduct(const Vector3 &v);
	Vector3 crossProduct(const Vector3 &v);

	Vector3 operator + (const Vector3 &v);
	Vector3 operator - (const Vector3 &v);
	Vector3 operator * (float scale);
	Vector3 operator / (float scale);
	Vector3 operator - ();
	bool operator == (const Vector3 &v);
};

class Vector3i {
public:
	int x, y, z;

	Vector3i() {
		x = -1;
		y = -1;
		z = -1;
	}

	Vector3i(int x, int y, int z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vector3i(const Vector3 &v) {
		x = v.x;
		y = v.y;
		z = v.z;
	}

	Vector3i(const Vector3i &v) {
		x = v.x;
		y = v.y;
		z = v.z;
	}

	Vector3i operator + (const Vector3i &v);
	Vector3i operator + (const Vector3 &v);
	Vector3i operator - (const Vector3i &v);
	Vector3i operator - (const Vector3 &v);
	bool operator == (const Vector3i &v);
	operator Vector3();
};