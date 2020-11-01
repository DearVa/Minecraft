#pragma once
#include <GL/glut.h>
#include <iostream>
#include <memory>

namespace file {
	GLuint LoadBMPTexture(const char *fileName);
	GLuint LoadBMPTexture(const char *fileName, const char *ofileName);  // Í¸Ã÷
}