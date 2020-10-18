#pragma once
#include <thread>
#include <iostream>
#include <Windows.h>
#include "vector2.h"
#include "world.h"

using namespace world;
namespace physics {
	struct RayCastHit {
		Vector3i pos;
		Vector3i face;
	};

	void Initial(float ms);
	void PhysicsLoop(float ms);
	RayCastHit *Raycast(Vector3 origin, Vector2 direction, GLfloat dis);
}