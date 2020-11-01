#pragma once
#include <ctime>
#include <thread>
#include <iostream>
#include <Windows.h>
#include "vector2.h"
#include "world.h"

namespace physics {
	using namespace world;
	struct RayCastHit {
		Vector3i pos;
		Vector3i face;
	};

	void Initial(long ms);
	void PhysicsLoop(long ms);
	RayCastHit *Raycast(Vector3 origin, Vector2 direction, GLfloat dis);
}