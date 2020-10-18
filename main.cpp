#include <GL/glut.h>
#include <Windows.h>
#include "graphic.h"
#include "physics.h"

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	int width = GetSystemMetrics(SM_CXSCREEN);
	int height = GetSystemMetrics(SM_CYSCREEN);
	graphic::Initial(width, height);
	player::Initial(width, height);
	world::Initial();
	physics::Initial(10.0f);
	glutMainLoop();
	return 0;
}