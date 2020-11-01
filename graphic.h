#pragma once
#include "everything.h"
#include <iostream>
#include <random>
#include <vector>
#include <ctime>

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

	struct ParticalNode {
		Partical *partical;
		ParticalNode *next;
	};

	extern GLfloat r;
	extern ParticalNode *particals, *particalsEnd;
	extern GLFWwindow *window;

	void ParticalEffect(Vector3i pos, GLuint *texs, int texsSize, int count, float maxVel, float lastTime, float g);
	void Display();
	void Initial(int width, int height);
	void RenderLoop();
}