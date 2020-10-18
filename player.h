#pragma once

#include <GL/glut.h>
#include <iostream>
#include <Windows.h>
#include <vector>
#include "vector3.h"
#include "vector2.h"
#include "world.h"
#include "physics.h"

using namespace std;
using namespace world;
namespace player {
	extern Vector3 pos;  // 玩家坐标
	extern GLdouble vx, vy, vz;  // 速度
	extern GLdouble vf, vr;  // 正向速度，右向速度
	extern Vector2 rot;  // 视角旋转
	extern bool grounded;  // 是否落地
	extern GLfloat xSensitivity, ySensitivity;  // 鼠标灵敏度
	extern GLdouble speed;  // 行走速度
	extern int hWidth, hHeight;  // 屏幕宽高一半
	extern float val;
	extern physics::RayCastHit *hit;

	void Initial(int width, int height);
	void KeyBoard(unsigned char key, int x, int y);
	void KeyBoardUp(unsigned char key, int x, int y);
	void Mouse(int button, int state, int x, int y);
	void MouseMove(int x, int y);
}