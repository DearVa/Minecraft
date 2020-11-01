#pragma once
#include <iostream>
#include <Windows.h>
#include <vector>
#include "physics.h"
#include "everything.h"

namespace player {
	extern Vector3 pos;  // �������
	extern GLdouble vx, vy, vz;  // �ٶ�
	extern bool w, s, a, d;  // �Ƿ���wsad
	extern GLdouble vf, vr;  // �����ٶȣ������ٶ�
	extern Vector2 rot;  // �ӽ���ת
	extern bool grounded;  // �Ƿ����
	extern GLfloat xSensitivity, ySensitivity;  // ���������
	extern GLdouble speed;  // �����ٶ�
	extern const GLdouble walkSpeed, runSpeed, crouchSpeed;
	extern int hWidth, hHeight;  // ��Ļ���һ��
	extern float val;
	extern physics::RayCastHit *hit;

	void Initial(int width, int height);
	void KeyBoard(GLFWwindow *window, int key, int scancode, int action, int mods);
	void Mouse(GLFWwindow *window, int button, int action, int mods);
	void MouseMove(GLFWwindow *window, double x, double y);
}