#pragma once
#include <iostream>
#include <memory>
#include "graphic.h"

namespace file {
	GLuint LoadBMPTexture(const char *fileName);
	GLuint LoadBMPTexture(const char *fileName, const char *ofileName);  // Í¸Ã÷
}