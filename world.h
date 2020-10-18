#pragma once
#include <GL/glut.h>
#include "block.h"
#include "vector3.h"

namespace world {
	struct BlockSet {  // Çø¿é
		int x, z;
		Block *blocks[32][16][16];
		BlockSet *f, *b, *l, *r;
	};

	enum class DIRECTION {
		FRONT, LEFT, BACK, RIGHT
	};

	extern BlockSet *worldCenter, *playerSet;

	void Initial();
	BlockSet *CreateBlockSet(BlockSet *blockSet, DIRECTION dir);
	BlockSet *GetBlockSet(int x, int z);
	void CreateWorld(Block *baseBlock);
	void DrawWorld(GLuint maxVision);
	void SetBlock(BlockSet *blockSet, int x, int y, int z, Block *block);
	void SetBlock(int x, int y, int z, Block *block);
	void SetBlock(Vector3i pos, Block *block);
	void PutBlock(Vector3i pos, Block *block);
	Block *GetBlock(int x, int y, int z);
	Block *GetBlock(Vector3i pos);
	void RemoveBlock(int x, int y, int z);
	void RemoveBlock(Vector3i pos);
	void DestoryBlock(Vector3i pos);
	bool GetCollider(int x, int y, int z);
}