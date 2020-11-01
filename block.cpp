#include "block.h"

Block::Block(GLuint tex, bool transparent) {
	this->transparent = transparent;
	for (int i = 0; i < 6; i++) {
		texs[i] = tex;
	}
}

Block::Block(GLuint texs[], bool transparent) {
	this->transparent = transparent;
	memcpy(this->texs, texs, 6 * sizeof(GLuint));
}

void Block::Draw(GLfloat x, GLfloat y, GLfloat z, bool *visible) {
	if (transparent) {
		glEnable(GL_BLEND);
	} else {
		glDisable(GL_BLEND);
	}
	glPushMatrix();
	glTranslatef(x, y, -z);
	for (int i = 0; i < 6; i++) {
		if (visible[i]) {
			glBindTexture(GL_TEXTURE_2D, texs[i]);
			glBegin(GL_QUADS);
			for (int j = 0; j < 4; j++) {
				glTexCoord2f(TexCoords[j][0], TexCoords[j][1]);
				int VtxId = cubeVi[i][j];
				glVertex3fv(cubeVs[VtxId]);
			}
			glEnd();
		}
	}
	glPopMatrix();
}

namespace blockMgr {
	GLuint STONE, GRASS_TOP, GRASS_SIDE, DIRT, TREE_TOP, TREE_SIDE, WOOD, LEAVE;
	Block *stone, *grassBlock, *dirt, *tree, *wood, *leave;

	void Initial() {
		STONE = LoadBMPTexture("./Tex/stone.bmp");
		GRASS_TOP = LoadBMPTexture("./Tex/grass_top.bmp");
		GRASS_SIDE = LoadBMPTexture("./Tex/grass_side.bmp");
		DIRT = LoadBMPTexture("./Tex/dirt.bmp");
		TREE_TOP = LoadBMPTexture("./Tex/tree.bmp");
		TREE_SIDE = LoadBMPTexture("./Tex/tree_side.bmp");
		WOOD = LoadBMPTexture("./Tex/wood.bmp");
		LEAVE = LoadBMPTexture("./Tex/leaves.bmp", "./Tex/leaves_opaque.bmp");

		stone = new Block(STONE, false);
		GLuint grassTexs[] = { GRASS_TOP, GRASS_SIDE, GRASS_SIDE, GRASS_SIDE, GRASS_SIDE, DIRT };
		grassBlock = new Block(grassTexs, false);
		dirt = new Block(DIRT, false);
		GLuint treeTexs[] = { TREE_TOP, TREE_SIDE, TREE_SIDE, TREE_SIDE, TREE_SIDE, TREE_TOP };
		tree = new Block(treeTexs, false);
		wood = new Block(WOOD, false);
		leave = new Block(LEAVE, true);
	}
}