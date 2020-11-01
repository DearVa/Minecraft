#pragma once
#include <thread>
#include <utility>
#include "block.h"
#include "graphic.h"
#include "vector3.h"
#include "vector2.h"
#include "perlinNoise.h"
#define VISION 3

namespace world {
	using namespace std;
	struct BlockInfo {
		Block *block;
		bool visible[6] = { true,true,true,true,true,true };
	};

	struct Chunk {  // 区块
		bool created = false;
		int x, z;
		BlockInfo blockInfos[128][16][16];
		int dirtHeight = 20;  // 泥土层高度
		int stoneHeight = 10;  // 石头层高度
		int baseHeight = 20;
	};

	enum class DIRECTION {
		FRONT, LEFT, BACK, RIGHT
	};

	extern int px, pz;
	extern long wpx, wpz;

	void Initial(double mapSeed);
	Chunk *GetChunkByChunkPos(int x, int z);
	void UpdateChunk();
	void UpdateChunkLoop(long ms);
	Chunk *GetChunkByWorldPos(int x, int z);
	void CreateTerrain(Chunk *chunk);
	void DrawWorld();
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