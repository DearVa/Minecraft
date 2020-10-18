#pragma once
#include <GL/glut.h>
#include <iostream>
#include <random>
#include <vector>
#include <ctime>
#include "player.h"
#include "block.h"
#include "world.h"
#include "file.h"

using namespace std;
using namespace world;
namespace graphic {
	struct Partical {
		Vector3 pos;
		Vector3 vel;  // 速度
		float g = 0;  // 重力
		float timeLeft = 0;
		float size = 1;
		GLuint tex;
		Vector2 texOffset;
	};

	extern GLfloat r;
	extern vector<graphic::Partical> particals;

	void ParticalEffect(Vector3i pos, GLuint *texs, int texsSize, int count, float maxVel, float lastTime, float g);
	void Display();
	void Initial(int width, int height);
}