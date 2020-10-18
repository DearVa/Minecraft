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
		Vector3 vel;  // �ٶ�
		float g = 0;  // ����
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

	void ParticalEffect(Vector3i pos, GLuint *texs, int texsSize, int count, float maxVel, float lastTime, float g);
	void Display();
	void Initial(int width, int height);
}