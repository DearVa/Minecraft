#pragma once
#include <GL/glut.h>
#include <memory>
#include "file.h"

const GLfloat cubeVs[8][3] = { {0.5,0.5,-0.5},{-0.5,0.5,-0.5},{-0.5,0.5,0.5},{0.5,0.5,0.5},{0.5,-0.5,0.5},{-0.5,-0.5,0.5},{-0.5,-0.5,-0.5},{0.5,-0.5,-0.5} }; //�����嶥��
const int cubeVi[6][4] = { {0,1,2,3},{0,3,4,7},{3,2,5,4},{2,1,6,5},{1,0,7,6},{6,7,4,5} }; //�����嶥�����к�
const GLfloat TexCoords[4][2] = { {0,1},{1,1},{1,0},{0,0} };

class Block {
public:
	GLuint texs[6];

	Block(GLuint tex);
	Block(GLuint texs[]);

	void Draw(GLfloat x, GLfloat y, GLfloat z);
};

using namespace file;
namespace blockMgr {
	extern GLuint STONE, GRASS_TOP, GRASS_SIDE, DIRT;
	extern Block *stone, *grassBlock, *dirt, *tree, *wood, *leave;

	void Initial();
}