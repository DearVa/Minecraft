#pragma once
#include <GL/glut.h>
#include <map>
#include <mutex>
#include <utility>
#include "block.h"
#include "vector3.h"
#include "vector2.h"
#include "perlinNoise.h"
#define VISION 5

namespace world {
	using namespace std;
	struct BlockInfo {
		Block *block;
		bool visible[6] = { true,true,true,true,true,true };
	};

	struct Chunk {  // 区块
		int x, z;
		BlockInfo blocks[128][16][16];
		int dirtHeight = 40;  // 泥土层高度
		int stoneHeight = 10;  // 石头层高度
		int baseHeight = 20;
	};

	enum class DIRECTION {
		FRONT, LEFT, BACK, RIGHT
	};

	typedef pair<int, int> v2;

	extern map<v2, Chunk*> worldMap;
	extern Chunk *worldCenter, *playerSet;

	void Initial(double mapSeed);
	void UpdateChunk();
	void UpdateChunkLoop(long ms);
	Chunk *CreateChunk(int x, int z);
	Chunk *CreateChunk(Chunk *chunk, DIRECTION dir);
	Chunk *GetChunk(int x, int z);
	void CreateWorld();
	void CreateTerrain(Chunk *chunk);
	void DrawWorld(int maxVision);
	void SetBlockVisible(int x, int y, int z, bool *visible);
	void SetChunkBlockVisiable(Chunk *chunk);
	void SetBlock(Chunk *chunk, int x, int y, int z, Block *block);
	void SetBlock(int x, int y, int z, Block *block);
	void SetBlock(Vector3i pos, Block *block);
	void PutBlock(Vector3i pos, Block *block);
	BlockInfo *GetBlockInfo(Chunk *chunk, int x, int y, int z);
	BlockInfo *GetBlockInfo(int x, int y, int z);
	BlockInfo *GetBlockInfo(Vector3i pos);
	void RemoveBlock(int x, int y, int z);
	void RemoveBlock(Vector3i pos);
	void DestoryBlock(Vector3i pos);
	bool GetCollider(int x, int y, int z);
}