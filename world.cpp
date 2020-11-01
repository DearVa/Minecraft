#include "world.h"
#include "graphic.h"

namespace world {
	int px, pz, opx, opz;
	long wpx, wpz;
	ImprovedNoise pNoise;
	double seed;
	Chunk worldChunks[2 * VISION + 1][2 * VISION + 1], updatedChunks[2 * VISION + 1][2 * VISION + 1];

	void Initial(double mapSeed) {
		seed = mapSeed;
		blockMgr::Initial();
		px = player::pos.x / 16;
		pz = player::pos.z / 16;
		wpx = px * 16;
		wpz = pz * 16;
		UpdateChunk();
		thread u = thread(UpdateChunkLoop, 500);
		u.detach();
	}

	Chunk *GetChunkByChunkPos(int x, int z) {
		if (VISION - opx + x < 0 || -opx + x > VISION || VISION - opz + z < 0 || -opz + z > VISION) {
			return nullptr;
		}
		return &worldChunks[VISION - opx + x][VISION - opz + z];
	}

	void UpdateChunk() {
		px = player::pos.x / 16;
		pz = player::pos.z / 16;
		if (px == opx && pz == opz) {
			return;
		}
		int dx = px - opx, dz = pz - opz;
		for (int x = -VISION; x <= VISION; x++) {
			for (int z = -VISION; z <= VISION; z++) {
				if (x + dx > VISION || VISION + x + dx < 0 || z + dz > VISION || VISION + z + dz < 0) {
					updatedChunks[VISION + x][VISION + z].created = false;
					updatedChunks[VISION + x][VISION + z].x = px + x;
					updatedChunks[VISION + x][VISION + z].z = pz + z;
				} else {
					updatedChunks[VISION + x][VISION + z] = worldChunks[VISION + x + dx][VISION + z + dz];
				}
			}
		}
		memcpy(worldChunks, updatedChunks, sizeof(updatedChunks));
		opx = px;
		opz = pz;
		wpx = px * 16;
		wpz = pz * 16;
		for (int x = -VISION; x <= VISION; x++) {
			for (int z = -VISION; z <= VISION; z++) {
				CreateTerrain(&worldChunks[VISION + x][VISION + z]);
			}
		}
		for (int x = -VISION; x <= VISION; x++) {
			for (int z = -VISION; z <= VISION; z++) {
				SetChunkBlockVisiable(&worldChunks[VISION + x][VISION + z]);
			}
		}
	}

	void UpdateChunkLoop(long ms) {
		while (true) {
			UpdateChunk();
			Sleep(ms);
		}
	}

	Chunk *GetChunkByWorldPos(int x, int z) {
		return GetChunkByChunkPos(x / 16, z / 16);
	}

	double inline fun(double t) {
		return t * t * t * (t * (6 * t - 15) + 10);
	}

	void CreateTerrain(Chunk *chunk) {
		if (chunk && !chunk->created) {
			for (int x = 0; x < 16; x++) {
				for (int z = 0; z < 16; z++) {
					double xx = (chunk->x + x / 16.0) / 3.0,
						zz = (chunk->z + z / 16.0) / 3.0;
					double sum = pNoise.noise(xx, zz, seed) + 1;
					int b1 = chunk->baseHeight + sum * chunk->dirtHeight - 1;
					for (int y = chunk->baseHeight; y < b1; y++) {
						chunk->blockInfos[y][z][x].block = blockMgr::dirt;
					}
					chunk->blockInfos[b1][z][x].block = blockMgr::grassBlock;
					sum = pNoise.noise(xx, zz, seed - 623321.41581) + 1; // 石头
					int b2 = sum * chunk->stoneHeight - 1;
					for (int y = chunk->baseHeight; y < b1 - b2; y++) {
						chunk->blockInfos[y][z][x].block = blockMgr::stone;
					}
				}
			}
		}
	}

	void DrawWorld() {
		Chunk *chunk;
		for (int x = -VISION; x <= VISION; x++) {
			for (int z = -VISION; z <= VISION; z++) {
				chunk = &worldChunks[VISION + x][VISION + z];
				if (chunk) {
					glPushMatrix();
					glTranslatef(16 * x, 0, -16 * z);
					for (int y = 0; y < 128; y++) {
						for (int z = 0; z < 16; z++) {
							for (int x = 0; x < 16; x++) {
								if (chunk->blockInfos[y][z][x].block != nullptr) {
									chunk->blockInfos[y][z][x].block->Draw(x, y, z, chunk->blockInfos[y][z][x].visible);
								}
							}
						}
					}
					glPopMatrix();
				}
			}
		}
	}

	void SetBlockVisible(int x, int y, int z, bool *visible) {
		BlockInfo *bi = GetBlockInfo(x, y, z);
		if (bi && bi->block) {
			bool t = GetBlockInfo(x, y, z)->block->transparent;
			BlockInfo *b1 = GetBlockInfo(x + 1, y, z);
			BlockInfo *b2 = GetBlockInfo(x, y + 1, z);
			BlockInfo *b3 = GetBlockInfo(x, y, z + 1);
			BlockInfo *b4 = GetBlockInfo(x - 1, y, z);
			BlockInfo *b5 = GetBlockInfo(x, y - 1, z);
			BlockInfo *b6 = GetBlockInfo(x, y, z - 1);
			visible[1] = !(b1 && b1->block) || (b1->block->transparent ^ t);
			visible[0] = !(b2 && b2->block) || (b2->block->transparent ^ t);
			visible[4] = !(b3 && b3->block) || (b3->block->transparent ^ t);
			visible[3] = !(b4 && b4->block) || (b4->block->transparent ^ t);
			visible[5] = !(b5 && b5->block) || (b5->block->transparent ^ t);
			visible[2] = !(b6 && b6->block) || (b6->block->transparent ^ t);
		}
	}

	void SetChunkBlockVisiable(Chunk *chunk) {
		if (chunk && !chunk->created) {
			for (int x = 0; x < 16; x++) {
				for (int z = 0; z < 16; z++) {
					for (int y = 1; y < 127; y++) {
						if (chunk->blockInfos[y][z][x].block) {
							SetBlockVisible(chunk->x * 16 + x, y, chunk->z * 16 + z, chunk->blockInfos[y][z][x].visible);
						}
					}
				}
			}
			chunk->created = true;
		}
	}

	void SetBlock(Chunk *chunk, int x, int y, int z, Block *block) {
		if (y < 0) {
			return;
		}
		if (chunk != nullptr) {
			chunk->blockInfos[y][z][x].block = block;
			int bx = chunk->x * 16, bz = chunk->z * 16;
			SetBlockVisible(bx + x, y, bz + z, chunk->blockInfos[y][z][x].visible);
			SetBlockVisible(bx + x + 1, y, bz + z, GetBlockInfo(bx + x + 1, y, bz + z)->visible);
			SetBlockVisible(bx + x, y + 1, bz + z, GetBlockInfo(bx + x, y + 1, bz + z)->visible);
			SetBlockVisible(bx + x, y, bz + z + 1, GetBlockInfo(bx + x, y, bz + z + 1)->visible);
			SetBlockVisible(bx + x - 1, y, bz + z, GetBlockInfo(bx + x - 1, y, bz + z)->visible);
			SetBlockVisible(bx + x, y - 1, bz + z, GetBlockInfo(bx + x, y - 1, bz + z)->visible);
			SetBlockVisible(bx + x, y, bz + z - 1, GetBlockInfo(bx + x, y, bz + z - 1)->visible);
		}
	}

	void SetBlock(int x, int y, int z, Block *block) {
		SetBlock(GetChunkByWorldPos(x, z), x % 16, y, z % 16, block);
	}

	void SetBlock(Vector3i pos, Block *block) {
		SetBlock(pos.x, pos.y, pos.z, block);
	}

	void PutBlock(Vector3i pos, Block *block) {  // 检测
		if (GetBlockInfo(pos)->block) {
			return;
		}
		if ((int)round(player::pos.x) == pos.x && (int)round(player::pos.z) == pos.z &&
		   ((int)round(player::pos.y) == pos.y || (int)round(player::pos.y) == pos.y + 1)) {
			return;
		}
		SetBlock(pos, block);
	}

	BlockInfo *GetBlockInfo(Chunk *chunk, int x, int y, int z) {
		if (y < 0) {
			return nullptr;
		}
		if (chunk != nullptr) {
			int xx = x % 16, zz = z % 16;
			return &chunk->blockInfos[y][zz][xx];
		}
		return nullptr;
	}

	BlockInfo *GetBlockInfo(int x, int y, int z) {
		if (y < 0) {
			return nullptr;
		}
		Chunk *chunk = GetChunkByWorldPos(x, z);
		return GetBlockInfo(chunk, x, y, z);
	}

	BlockInfo *GetBlockInfo(Vector3i pos) {
		return GetBlockInfo(pos.x, pos.y, pos.z);
	}

	void RemoveBlock(int x, int y, int z) {
		if (y < 0) {
			return;
		}
		Chunk *chunk = GetChunkByWorldPos(x, z);
		if (chunk != nullptr) {
			int xx = x % 16, zz = z % 16;
			chunk->blockInfos[y][zz][xx].block = nullptr;
			int bx = chunk->x * 16, bz = chunk->z * 16;
			SetBlockVisible(bx + xx + 1, y, bz + zz, GetBlockInfo(bx + xx + 1, y, bz + zz)->visible);
			SetBlockVisible(bx + xx, y + 1, bz + zz, GetBlockInfo(bx + xx, y + 1, bz + zz)->visible);
			SetBlockVisible(bx + xx, y, bz + zz + 1, GetBlockInfo(bx + xx, y, bz + zz + 1)->visible);
			SetBlockVisible(bx + xx - 1, y, bz + zz, GetBlockInfo(bx + xx - 1, y, bz + zz)->visible);
			SetBlockVisible(bx + xx, y - 1, bz + zz, GetBlockInfo(bx + xx, y - 1, bz + zz)->visible);
			SetBlockVisible(bx + xx, y, bz + zz - 1, GetBlockInfo(bx + xx, y, bz + zz - 1)->visible);
		}
	}

	void RemoveBlock(Vector3i pos) {
		RemoveBlock(pos.x, pos.y, pos.z);
	}

	void DestoryBlock(Vector3i pos) {  // 带有粒子效果
		graphic::ParticalEffect(pos, GetBlockInfo(pos)->block->texs, 6, 10, 5, 0.8f, 20);
		RemoveBlock(pos);
	}

	bool GetCollider(int x, int y, int z) {
		if (y < 0) {
			return false;
		}
		Chunk *chunk = GetChunkByWorldPos(x, z);
		if (chunk != nullptr) {
			return chunk->blockInfos[y][z % 16][x % 16].block || chunk->blockInfos[y - 1][z % 16][x % 16].block;
		}
		return true;
	}
}