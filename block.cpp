#include "block.h"

Block::Block(GLuint tex) {
	for (int i = 0; i < 6; i++) {
		texs[i] = tex;
	}
}

Block::Block(GLuint texs[]) {
	memcpy(this->texs, texs, 6 * sizeof(GLuint));
}

void Block::Draw(GLfloat x, GLfloat y, GLfloat z) {
	glPushMatrix();
	glTranslatef(x, y, -z);
	for (int i = 0; i < 6; i++) {
		glBindTexture(GL_TEXTURE_2D, texs[i]);
		glBegin(GL_QUADS);
		for (int j = 0; j < 4; j++) {
			glTexCoord2f(TexCoords[j][0], TexCoords[j][1]);
			int VtxId = cubeVi[i][j];
			glVertex3fv(cubeVs[VtxId]);
		}
		glEnd();
	}
	glPopMatrix();
}

namespace blockMgr {
	GLuint STONE, GRASS_TOP, GRASS_SIDE, DIRT, TREE_TOP, TREE_SIDE, WOOD;
	Block *stone, *grassBlock, *dirt, *tree, *wood;

	void Initial() {
		STONE = LoadBMPTexture("./Tex/stone.bmp");
		GRASS_TOP = LoadBMPTexture("./Tex/grass_top.bmp");
		GRASS_SIDE = LoadBMPTexture("./Tex/grass_side.bmp");
		DIRT = LoadBMPTexture("./Tex/dirt.bmp");
		TREE_TOP = LoadBMPTexture("./Tex/tree.bmp");
		TREE_SIDE = LoadBMPTexture("./Tex/tree_side.bmp");
		WOOD = LoadBMPTexture("./Tex/wood.bmp");

		stone = new Block(STONE);
		GLuint grassTexs[] = { GRASS_TOP, GRASS_SIDE, GRASS_SIDE, GRASS_SIDE, GRASS_SIDE, DIRT };
		grassBlock = new Block(grassTexs);
		dirt = new Block(DIRT);
		GLuint treeTexs[] = { TREE_TOP, TREE_SIDE, TREE_SIDE, TREE_SIDE, TREE_SIDE, TREE_TOP };
		tree = new Block(treeTexs);
		wood = new Block(WOOD);
	}
}