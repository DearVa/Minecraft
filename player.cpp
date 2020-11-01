#include "player.h"
#include "world.h"

namespace player {
	using namespace std;
	using namespace world;

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

	void KeyBoard(GLFWwindow *window, int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_W) {
			w = action;
		} 
		if (key == GLFW_KEY_S) {
			s = action;
		}
		if (key == GLFW_KEY_A) {
			a = action;
		}
		if (key == GLFW_KEY_D) {
			d = action;
		}
		if (key == GLFW_KEY_SPACE && grounded && action) {  // ��Ծ
			vy += 9;
		}
		if (key == GLFW_KEY_ESCAPE) {  // ESC
			glfwSetWindowShouldClose(window, true);
		}
	}

	void Mouse(GLFWwindow *window, int button, int action, int mods) {
		if (button == GLFW_MOUSE_BUTTON_1 && action) {
			if (hit != nullptr) {
				DestoryBlock(hit->pos);
			}
		} else if (button == GLFW_MOUSE_BUTTON_2 && action) {
			if (hit != nullptr) {
				PutBlock(hit->pos + hit->face, blockMgr::leave);
			}
		}
	}

	void MouseMove(GLFWwindow *window, double x, double y) {
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