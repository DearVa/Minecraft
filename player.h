#pragma once
#include <iostream>
#include <Windows.h>
#include <vector>
#include "physics.h"
#include "everything.h"

namespace player {
	extern Vector3 pos;  // 玩家坐标
	extern GLdouble vx, vy, vz;  // 速度
	extern bool w, s, a, d;  // 是否按下wsad
	extern GLdouble vf, vr;  // 正向速度，右向速度
	extern Vector2 rot;  // 视角旋转
	extern bool grounded;  // 是否落地
	extern GLfloat xSensitivity, ySensitivity;  // 鼠标灵敏度
	extern GLdouble speed;  // 行走速度
	extern const GLdouble walkSpeed, runSpeed, crouchSpeed;
	extern int hWidth, hHeight;  // 屏幕宽高一半
	extern float val;
	extern physics::RayCastHit *hit;

	void Initial(int width, int height);
	void KeyBoard(GLFWwindow *window, int key, int scancode, int action, int mods);
	void Mouse(GLFWwindow *window, int button, int action, int mods);
	void MouseMove(GLFWwindow *window, double x, double y);
}