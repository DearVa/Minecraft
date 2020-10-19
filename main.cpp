#include <GL/glut.h>
#include <Windows.h>
#include "graphic.h"
#include "physics.h"
#include "perlinNoise.h"

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	int width = GetSystemMetrics(SM_CXSCREEN);
	int height = GetSystemMetrics(SM_CYSCREEN);
	graphic::Initial(width, height);
	player::Initial(width, height);
	world::Initial(7.89101112131415);
	physics::Initial(10);
	glutMainLoop();
	return 0;
}