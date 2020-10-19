#pragma once
#include <GL/glut.h>
#include "block.h"
#include "vector3.h"

namespace world {
	struct BlockInfo {
		Block *block;
		bool visible;
	};

	struct BlockSet {  // Çø¿é
		int x, z;
		BlockInfo blocks[128][16][16];
		BlockSet *f, *b, *l, *r;
	};

	enum class DIRECTION {
		FRONT, LEFT, BACK, RIGHT
	};

	extern BlockSet *worldCenter, *playerSet;

	void Initial(double mapSeed);
	BlockSet *CreateBlockSet(BlockSet *blockSet, DIRECTION dir);
	BlockSet *GetBlockSet(int x, int z);
	void CreateWorld(Block *baseBlock);
	void CreateTerrain(BlockSet *blockSet);
	void DrawWorld(GLuint maxVision);
	bool BlockVisible(int x, int y, int z);
	void SetBlockSetBlockVisiable(BlockSet *blockSet);
	void SetBlock(BlockSet *blockSet, int x, int y, int z, Block *block);
	void SetBlock(int x, int y, int z, Block *block);
	void SetBlock(Vector3i pos, Block *block);
	void PutBlock(Vector3i pos, Block *block);
	BlockInfo *GetBlock(BlockSet *blockSet, int x, int y, int z);
	BlockInfo *GetBlock(int x, int y, int z);
	BlockInfo *GetBlock(Vector3i pos);
	void RemoveBlock(int x, int y, int z);
	void RemoveBlock(Vector3i pos);
	void DestoryBlock(Vector3i pos);
	bool GetCollider(int x, int y, int z);
}