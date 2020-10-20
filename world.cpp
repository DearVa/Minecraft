#include "world.h"
#include "graphic.h"

namespace world {
	map<v2, Chunk*> worldMap;
	Chunk *worldCenter, *playerSet;
	ImprovedNoise pNoise;
	double seed;
	Chunk *bs[VISION * VISION - 1];
	//mutex mlock;

	void Initial(double mapSeed) {
		seed = mapSeed;
		blockMgr::Initial();
		CreateWorld();
		//thread u = thread(UpdateChunkLoop, 500);
		//u.detach();
	}

	void UpdateChunk() {
		//mlock.lock();
		map<v2, Chunk*>::iterator it;
		memset(bs, 0, sizeof(bs));
		playerSet = GetChunk(player::pos.x, player::pos.z);
		if (!playerSet) {
			playerSet = CreateChunk(player::pos.x / 16, player::pos.z / 16);
		}
		int i = 0;
		for (int x = -VISION; x <= VISION; x++) {
			for (int z = -VISION; z <= VISION; z++) {
				it = worldMap.find(v2(playerSet->x + x, playerSet->z + z));
				if (it == worldMap.end()) {
					bs[i++] = CreateChunk(playerSet->x + x, playerSet->z + z);
				}
			}
		}
		for (i = 0; i < VISION * VISION - 1; i++) {
			if (bs[i]) {
				CreateTerrain(bs[i]);
			}
		}
		for (i = 0; i < VISION * VISION - 1; i++) {
			if (bs[i]) {
				SetChunkBlockVisiable(bs[i]);
			}
		}
		//mlock.unlock();
	}

	void UpdateChunkLoop(long ms) {
		while (true) {
			UpdateChunk();
			Sleep(ms);
		}
	}

	Chunk *GetChunk(int x, int z) {
		//mlock.lock();
		int xx = x / 16, zz = z / 16;
		if (playerSet && playerSet->x == xx && playerSet->z == zz) {
			return playerSet;
		}
		for (int i = 0; i < 8; i++) {
			if (bs[i] && bs[i]->x == xx && bs[i]->z == zz) {
				return bs[i];
			}
		}
		map<v2, Chunk*>::iterator it = worldMap.find(v2(xx, zz));
		if (it != worldMap.end()) {
			return (*it).second;
		} else {
			return nullptr;
		}
		//mlock.unlock();
	}

	Chunk *CreateChunk(int x, int z) {
		map<v2, Chunk*>::iterator it = worldMap.find(v2(x, z));
		if (it != worldMap.end()) {
			return (*it).second;
		} else {
			Chunk *nbs = new Chunk;
			memset(nbs->blocks, 0, sizeof(worldCenter->blocks));
			nbs->x = x;
			nbs->z = z;
			worldMap[v2(nbs->x, nbs->z)] = nbs;
			return nbs;
		}
	}

	Chunk *CreateChunk(Chunk *chunk, DIRECTION dir) {
		Chunk *nbs = new Chunk;
		memset(nbs->blocks, 0, sizeof(worldCenter->blocks));
		map<v2, Chunk*>::iterator it;
		if (dir == DIRECTION::FRONT) {
			it = worldMap.find(v2(chunk->x, chunk->z + 1));
			if (it != worldMap.end()) {
				return (*it).second;
			} else {
				nbs->x = chunk->x;
				nbs->z = chunk->z + 1;
				//mlock.lock();
				worldMap[v2(nbs->x, nbs->z)] = nbs;
				//mlock.unlock();
			}
		} else if (dir == DIRECTION::LEFT) {
			it = worldMap.find(v2(chunk->x - 1, chunk->z));
			if (it != worldMap.end()) {
				return (*it).second;
			} else {
				nbs->x = chunk->x - 1;
				nbs->z = chunk->z;
				//mlock.lock();
				worldMap[v2(nbs->x, nbs->z)] = nbs;
				//mlock.unlock();
			}
		} else if (dir == DIRECTION::BACK) {
			it = worldMap.find(v2(chunk->x, chunk->z - 1));
			if (it != worldMap.end()) {
				return (*it).second;
			} else {
				nbs->x = chunk->x;
				nbs->z = chunk->z - 1;
				//mlock.lock();
				worldMap[v2(nbs->x, nbs->z)] = nbs;
				//mlock.unlock();
			}
		} else {
			it = worldMap.find(v2(chunk->x + 1, chunk->z));
			if (it != worldMap.end()) {
				return (*it).second;
			} else {
				nbs->x = chunk->x + 1;
				nbs->z = chunk->z;
				//mlock.lock();
				worldMap[v2(nbs->x, nbs->z)] = nbs;
				//mlock.unlock();
			}
		}
		return nbs;
	}

	double inline fun(double t) {
		return t * t * t * (t * (6 * t - 15) + 10);
	}

	void CreateTerrain(Chunk *chunk) {
		if (chunk) {
			for (int x = 0; x < 16; x++) {
				for (int z = 0; z < 16; z++) {
					double xx = (chunk->x + x / 16.0) / 3.0,
						zz = (chunk->z + z / 16.0) / 3.0;
					double sum = pNoise.noise(xx, zz, seed) + 1;
					int b1 = chunk->baseHeight + sum * chunk->dirtHeight - 1;
					for (int y = chunk->baseHeight; y < b1; y++) {
						chunk->blocks[y][z][x].block = blockMgr::dirt;
					}
					chunk->blocks[b1][z][x].block = blockMgr::grassBlock;
					sum = pNoise.noise(xx, zz, seed - 623321.41581) + 1; // 石头
					int b2 = sum * chunk->stoneHeight - 1;
					for (int y = chunk->baseHeight; y < b1 - b2; y++) {
						chunk->blocks[y][z][x].block = blockMgr::stone;
					}
				}
			}
		}
	}

	void CreateWorld() {
		worldCenter = CreateChunk(6250000, 6250000);
		playerSet = worldCenter;
		Chunk *nbsl = CreateChunk(worldCenter, DIRECTION::LEFT);
		Chunk *nbs = CreateChunk(nbsl, DIRECTION::FRONT);
		nbs = CreateChunk(nbsl, DIRECTION::BACK);
		Chunk *nbsr = CreateChunk(worldCenter, DIRECTION::RIGHT);
		nbs = CreateChunk(nbsr, DIRECTION::FRONT);
		nbs = CreateChunk(nbsr, DIRECTION::BACK);
		nbs = CreateChunk(worldCenter, DIRECTION::FRONT);
		nbs = CreateChunk(worldCenter, DIRECTION::BACK);

		nbs = playerSet;
		map<v2, Chunk*>::iterator it;
		for (int z = nbs->z - 1; z < nbs->z + 2; z++) {
			for (int x = nbs->x - 1; x < nbs->x + 2; x++) {
				it = worldMap.find(v2(x, z));
				if (it != worldMap.end()) {
					CreateTerrain((*it).second);
				}
			}
		}

		for (int z = nbs->z - 1; z < nbs->z + 2; z++) {
			for (int x = nbs->x - 1; x < nbs->x + 2; x++) {
				it = worldMap.find(v2(x, z));
				if (it != worldMap.end()) {
					SetChunkBlockVisiable((*it).second);
				}
			}
		}
	}

	void DrawWorld(int maxVision) {
		//mlock.lock();
		Chunk *nbs = playerSet;
		if (nbs) {
			for (int zz = -maxVision; zz <= maxVision; zz++) {
				for (int xx = -maxVision; xx <= maxVision; xx++) {
					map<v2, Chunk*>::iterator it = worldMap.find(v2(xx + nbs->x, zz + nbs->z));
					if (it != worldMap.end()) {
						Chunk *bs = (*it).second;
						glPushMatrix();
						glTranslatef(16 * xx, 0, -16 * zz);
						for (int y = 0; y < 128; y++) {
							for (int z = 0; z < 16; z++) {
								for (int x = 0; x < 16; x++) {
									if (bs->blocks[y][z][x].block != nullptr) {
										bs->blocks[y][z][x].block->Draw(x, y, z, bs->blocks[y][z][x].visible);
									}
								}
							}
						}
						glPopMatrix();
					}
				}
			}
		}
		//mlock.unlock();
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
		for (int x = 0; x < 16; x++) {
			for (int z = 0; z < 16; z++) {
				for (int y = 1; y < 127; y++) {
					if (chunk->blocks[y][z][x].block) {
						SetBlockVisible(chunk->x * 16 + x, y, chunk->z * 16 + z, chunk->blocks[y][z][x].visible);
					}
				}
			}
		}
	}

	void SetBlock(Chunk *chunk, int x, int y, int z, Block *block) {
		if (y < 0) {
			return;
		}
		if (chunk != nullptr) {
			chunk->blocks[y][z][x].block = block;
			int bx = chunk->x * 16, bz = chunk->z * 16;
			SetBlockVisible(bx + x, y, bz + z, chunk->blocks[y][z][x].visible);
			SetBlockVisible(bx + x + 1, y, bz + z, GetBlockInfo(bx + x + 1, y, bz + z)->visible);
			SetBlockVisible(bx + x, y + 1, bz + z, GetBlockInfo(bx + x, y + 1, bz + z)->visible);
			SetBlockVisible(bx + x, y, bz + z + 1, GetBlockInfo(bx + x, y, bz + z + 1)->visible);
			SetBlockVisible(bx + x - 1, y, bz + z, GetBlockInfo(bx + x - 1, y, bz + z)->visible);
			SetBlockVisible(bx + x, y - 1, bz + z, GetBlockInfo(bx + x, y - 1, bz + z)->visible);
			SetBlockVisible(bx + x, y, bz + z - 1, GetBlockInfo(bx + x, y, bz + z - 1)->visible);
		}
	}

	void SetBlock(int x, int y, int z, Block *block) {
		SetBlock(GetChunk(x, z), x % 16, y, z % 16, block);
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
			return &chunk->blocks[y][zz][xx];
		}
		return nullptr;
	}

	BlockInfo *GetBlockInfo(int x, int y, int z) {
		if (y < 0) {
			return nullptr;
		}
		Chunk *chunk = GetChunk(x, z);
		return GetBlockInfo(chunk, x, y, z);
	}

	BlockInfo *GetBlockInfo(Vector3i pos) {
		return GetBlockInfo(pos.x, pos.y, pos.z);
	}

	void RemoveBlock(int x, int y, int z) {
		if (y < 0) {
			return;
		}
		Chunk *chunk = GetChunk(x, z);
		if (chunk != nullptr) {
			int xx = x % 16, zz = z % 16;
			chunk->blocks[y][zz][xx].block = nullptr;
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
		Chunk *chunk = GetChunk(x, z);
		if (chunk != nullptr) {
			return chunk->blocks[y][z % 16][x % 16].block || chunk->blocks[y - 1][z % 16][x % 16].block;
		}
		return false;
	}
}