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
		float drag = 0;  // 阻力
		float g = 0;  // 重力
		float timeLeft = 0;
		float size = 1;
		bool collision = false;
		GLuint tex;
		Vector2 texOffset;
	};

	struct ParticalNode {
		Partical *partical;
		ParticalNode *next;
	};

	struct MotionObj {
		Vector3 pos;
		Vector3 vel;  // 速度
		float timeLeft = 0;
		float size = 1;
		int type;
		GLuint tex;
		void OnDeath();
	};

	struct MotionObjNode {
		MotionObj *motionObj;
		MotionObjNode *next;
	};

	extern GLfloat r;
	extern ParticalNode *particals, *particalsEnd;
	extern MotionObjNode *motionObjs, *motionObjsEnd;

	void ParticalEffect(Vector3i pos, GLuint *texs, int texsSize, int count, float maxVel, float lastTime, float g, float drag, bool collision);
	void ParticalEffect(Vector3i pos, GLuint tex, int count, float maxVel, float lastTime, float g, float drag, bool collision);
	void FireworkEffect(Vector3i pos, GLuint tex, float lastTime, float g, float drag);
	void Summon(MotionObj *obj);
	void Display();
	void Initial(int width, int height);
}