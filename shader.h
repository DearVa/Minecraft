#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "graphic.h"

using namespace std;
class Shader {
public:
	Shader(const string &vsFile, const string &fsFile);
	void Use();
	void SetBool(const string &name, bool value) const;
	void SetInt(const string &name, int value) const;
	void SetFloat(const string &name, float value) const;
	~Shader();
private:
	GLuint program = 0;
	void CheckCompileErrors(GLuint shader, bool isProgram);
};