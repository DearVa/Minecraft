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
	extern Vector3 pos;  // �������
	extern GLdouble vx, vy, vz;  // �ٶ�
	extern GLdouble vf, vr;  // �����ٶȣ������ٶ�
	extern Vector2 rot;  // �ӽ���ת
	extern bool grounded;  // �Ƿ����
	extern GLfloat xSensitivity, ySensitivity;  // ���������
	extern GLdouble speed;  // �����ٶ�
	extern int hWidth, hHeight;  // ��Ļ���һ��
	extern float val;
	extern physics::RayCastHit *hit;

	void Initial(int width, int height);
	void KeyBoard(unsigned char key, int x, int y);
	void KeyBoardUp(unsigned char key, int x, int y);
	void Mouse(int button, int state, int x, int y);
	void MouseMove(int x, int y);
}