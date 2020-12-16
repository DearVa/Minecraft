#include "player.h"
#include "graphic.h"

namespace player {
	Vector3 pos = Vector3(100000000, 68, 100000000);  // �������
	GLdouble vx, vy, vz;  // �ٶ�
	bool w, s, a, d;
	GLdouble vf, vr;  // �����ٶȣ������ٶ�
	Vector2 rot;  // �ӽ���ת
	bool grounded;  // �Ƿ����
	GLfloat xSensitivity = 0.1f, ySensitivity = 0.1f;  // ���������
	GLdouble speed = 6;  // �����ٶ�
	const GLdouble walkSpeed = 6, runSpeed = 10, crouchSpeed = 3;
	int hWidth, hHeight;  // ��Ļ���һ��
	float val = 3.1415926535 / 180;
	physics::RayCastHit *hit = nullptr;

	void Initial(int width, int height) {
		hWidth = width / 2;
		hHeight = height / 2;
		SetCursorPos(hWidth, hHeight);
	}

	void KeyBoard(unsigned char key, int x, int y) {
		if (key == 'w' || key == 'W') {
			w = true;
		} 
		if (key == 's' || key == 'S') {
			s = true;
		}
		if (key == 'a' || key == 'A') {
			a = true;
		}
		if (key == 'd' || key == 'D') {
			d = true;
		}
		if (key == 32 && grounded) {  // ��Ծ
			vy += 9;
		}
		if (key == 27) {  // ESC
			exit(0);
		}
	}

	void KeyBoardUp(unsigned char key, int x, int y) {
		if (key == 'w' || key == 'W') {
			w = false;
		}
		if (key == 's' || key == 'S') {
			s = false;
		}
		if (key == 'a' || key == 'A') {
			a = false;
		}
		if (key == 'd' || key == 'D') {
			d = false;
		}
	}

	void Mouse(int button, int state, int x, int y) {
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
			if (hit != nullptr) {
				DestoryBlock(hit->pos);
			}
		} else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
			if (hit != nullptr) {
				graphic::MotionObj *obj = new graphic::MotionObj();
				obj->pos = hit->pos + hit->face;
				obj->tex = blockMgr::FIREWORK;
				obj->vel = Vector3(0, 8, 0);
				obj->timeLeft = 1.2;
				graphic::Summon(obj);
				// PutBlock(hit->pos + hit->face, blockMgr::stone);
			}
		}
	}

	void MouseMove(int x, int y) {
		rot.x += (x - hWidth) * xSensitivity;
		rot.y += (y - hHeight) * ySensitivity;
		if (rot.x > 360 || rot.x < -360) {
			rot.x = (int)rot.x % 360;
		}
		if (rot.y > 90) {
			rot.y = 90;
		} else if (rot.y < -90) {
			rot.y = -90;
		}
		SetCursorPos(hWidth, hHeight);

		hit = physics::Raycast(pos, rot, 10);
	}
}