#include "world.h"
#include "graphic.h"
#include "perlinNoise.h"

namespace world {
	BlockSet *worldCenter, *playerSet;
	ImprovedNoise pNoise;
	double seed;

	void Initial(double mapSeed) {
		using namespace blockMgr;
		seed = mapSeed;
		blockMgr::Initial();
		CreateWorld(grassBlock);
		//SetBlock(playerSet, 6, 2, 3, stone);
		//SetBlock(playerSet, 5, 1, 3, stone);
		//SetBlock(playerSet, 5, 1, 4, stone);
		//SetBlock(playerSet, 6, 3, 3, stone);
		//SetBlock(playerSet, 6, 4, 4, stone);
		//SetBlock(playerSet, 6, 3, 5, stone);
		//SetBlock(playerSet, 5, 2, 5, stone);
		//SetBlock(playerSet, 6, 4, 3, stone);
		//SetBlock(playerSet, 6, 5, 3, stone);
		//SetBlock(playerSet, 0, 2, 4, stone);
		//SetBlock(playerSet, 5, 2, 7, dirt);
		//SetBlock(playerSet, 6, 1, 4, dirt);
		//SetBlock(playerSet, 6, 4, 8, dirt);
		//SetBlock(playerSet, 5, 2, 8, dirt);
	}

	BlockSet *GetBlockSet(int x, int z) {
		int xx = x / 16, zz = z / 16;
		BlockSet *blockSet = playerSet;
		while (blockSet->l && xx < blockSet->x) {
			blockSet = blockSet->l;
		}
		while (blockSet->r && xx > blockSet->x) {
			blockSet = blockSet->r;
		}
		while (blockSet->b && zz < blockSet->z) {
			blockSet = blockSet->b;
		}
		while (blockSet->f && zz > blockSet->z) {
			blockSet = blockSet->f;
		}
		return blockSet;
	}

	BlockSet *CreateBlockSet(BlockSet *blockSet, DIRECTION dir) {
		BlockSet *nbs = new BlockSet;
		memset(nbs->blocks, 0, sizeof(worldCenter->blocks));
		nbs->f = nullptr;
		nbs->l = nullptr;
		nbs->b = nullptr;
		nbs->r = nullptr;
		if (dir == DIRECTION::FRONT) {
			if (blockSet->f != nullptr) {
				return blockSet->f;
			} else {
				nbs->x = blockSet->x;
				nbs->z = blockSet->z + 1;
				blockSet->f = nbs;
				nbs->b = blockSet;
				if (blockSet->l) {
					nbs->l = blockSet->l->f;
					if (nbs->l) {
						blockSet->l->f->r = nbs;
					}
				}
				if (blockSet->r) {
					nbs->r = blockSet->r->f;
					if (nbs->l) {
						blockSet->r->f->l = nbs;
					}
				}
			}
		} else if (dir == DIRECTION::LEFT) {
			if (blockSet->l != nullptr) {
				return blockSet->l;
			} else {
				nbs->x = blockSet->x - 1;
				nbs->z = blockSet->z;
				blockSet->l = nbs;
				nbs->r = blockSet;
				if (blockSet->b) {
					nbs->b = blockSet->b->l;
					if (nbs->b) {
						blockSet->b->l->f = nbs;
					}
				}
				if (blockSet->f) {
					nbs->f = blockSet->f->l;
					if (nbs->f) {
						blockSet->f->l->b = nbs;
					}
				}
			}
		} else if (dir == DIRECTION::BACK) {
			if (blockSet->b != nullptr) {
				return blockSet->b;
			} else {
				nbs->x = blockSet->x;
				nbs->z = blockSet->z - 1;
				blockSet->b = nbs;
				nbs->f = blockSet;
				if (blockSet->l) {
					nbs->l = blockSet->l->b;
					if (nbs->l) {
						blockSet->l->b->r = nbs;
					}
				}
				if (blockSet->r) {
					nbs->r = blockSet->r->b;
					if (nbs->l) {
						blockSet->r->b->l = nbs;
					}
				}
			}
		} else {
			if (blockSet->r != nullptr) {
				return blockSet->r;
			} else {
				nbs->x = blockSet->x + 1;
				nbs->z = blockSet->z;
				blockSet->r = nbs;
				nbs->l = blockSet;
				if (blockSet->b) {
					nbs->b = blockSet->b->r;
					if (nbs->b) {
						blockSet->b->r->f = nbs;
					}
				}
				if (blockSet->f) {
					nbs->f = blockSet->f->r;
					if (nbs->f) {
						blockSet->f->r->b = nbs;
					}
				}
			}
		}
		return nbs;
	}

	void CreateTerrain(BlockSet *blockSet) {
		for (int x = 0; x < 16; x++) {
			for (int z = 0; z < 16; z++) {
				double xx = blockSet->x + x / 16.0,
					zz = blockSet->z + z / 16.0;
				double sum = pNoise.noise(xx, zz, seed);
				int b = (sum + 1) * 32;
				for (int y = 0; y < b - 1; y++) {
					blockSet->blocks[y][z][x].block = blockMgr::dirt;
				}
				blockSet->blocks[b - 1][z][x].block = blockMgr::grassBlock;
			}
		}
	}

	void CreateWorld(Block *baseBlock) {
		worldCenter = new BlockSet;
		worldCenter->x = 6250000;
		worldCenter->z = 6250000;
		memset(worldCenter->blocks, 0, sizeof(worldCenter->blocks));
		worldCenter->f = nullptr;
		worldCenter->l = nullptr;
		worldCenter->b = nullptr;
		worldCenter->r = nullptr;
		CreateTerrain(worldCenter);
		playerSet = worldCenter;
		BlockSet *nbsl = CreateBlockSet(worldCenter, DIRECTION::LEFT);
		CreateTerrain(nbsl);
		BlockSet *nbs = CreateBlockSet(nbsl, DIRECTION::FRONT);
		for (int xx = 0; xx < 16; xx++) {
			for (int yy = 0; yy < 16; yy++) {
				nbs->blocks[0][xx][yy].block = baseBlock;
			}
		}
		CreateTerrain(nbs);
		nbs = CreateBlockSet(nbsl, DIRECTION::BACK);
		for (int xx = 0; xx < 16; xx++) {
			for (int yy = 0; yy < 16; yy++) {
				nbs->blocks[0][xx][yy].block = baseBlock;
			}
		}
		CreateTerrain(nbs);
		BlockSet *nbsr = CreateBlockSet(worldCenter, DIRECTION::RIGHT);
		for (int xx = 0; xx < 16; xx++) {
			for (int yy = 0; yy < 16; yy++) {
				nbsr->blocks[0][xx][yy].block = baseBlock;
			}
		}
		CreateTerrain(nbsr);
		nbs = CreateBlockSet(nbsr, DIRECTION::FRONT);
		for (int xx = 0; xx < 16; xx++) {
			for (int yy = 0; yy < 16; yy++) {
				nbs->blocks[0][xx][yy].block = baseBlock;
			}
		}
		CreateTerrain(nbs);
		nbs = CreateBlockSet(nbsr, DIRECTION::BACK);
		for (int xx = 0; xx < 16; xx++) {
			for (int yy = 0; yy < 16; yy++) {
				nbs->blocks[0][xx][yy].block = baseBlock;
			}
		}
		CreateTerrain(nbs);
		nbs = CreateBlockSet(worldCenter, DIRECTION::FRONT);
		for (int xx = 0; xx < 16; xx++) {
			for (int yy = 0; yy < 16; yy++) {
				nbs->blocks[0][xx][yy].block = baseBlock;
			}
		}
		CreateTerrain(nbs);
		nbs = CreateBlockSet(worldCenter, DIRECTION::BACK);
		for (int xx = 0; xx < 16; xx++) {
			for (int yy = 0; yy < 16; yy++) {
				nbs->blocks[0][xx][yy].block = baseBlock;
			}
		}
		CreateTerrain(nbs);

		nbs = playerSet;
		SetBlockSetBlockVisiable(nbs);
		nbs = nbs->l;
		SetBlockSetBlockVisiable(nbs);
		nbs = nbs->b;
		SetBlockSetBlockVisiable(nbs);
		nbs = nbs->r;
		SetBlockSetBlockVisiable(nbs);
		nbs = nbs->r;
		SetBlockSetBlockVisiable(nbs);
		nbs = nbs->f;
		SetBlockSetBlockVisiable(nbs);
		nbs = nbs->f;
		SetBlockSetBlockVisiable(nbs);
		nbs = nbs->l;
		SetBlockSetBlockVisiable(nbs);
		nbs = nbs->l;
		SetBlockSetBlockVisiable(nbs);
	}

	void DrawWorld(GLuint maxVision) {
		if (!playerSet) {
			return;
		}
		BlockSet *nbs = playerSet;
		for (int y = 0; y < 128; y++) {
			for (int z = 0; z < 16; z++) {
				for (int x = 0; x < 16; x++) {
					if (nbs->blocks[y][z][x].block != nullptr && nbs->blocks[y][z][x].visible) {
						nbs->blocks[y][z][x].block->Draw(x, y, z);
					}
				}
			}
		}
		nbs = nbs->l;
		glPushMatrix();
		glTranslatef(-16, 0, 0);
		for (int y = 0; y < 128; y++) {
			for (int z = 0; z < 16; z++) {
				for (int x = 0; x < 16; x++) {
					if (nbs->blocks[y][z][x].block != nullptr && nbs->blocks[y][z][x].visible) {
						nbs->blocks[y][z][x].block->Draw(x, y, z);
					}
				}
			}
		}
		glPopMatrix();
		nbs = nbs->b;
		glPushMatrix();
		glTranslatef(-16, 0, 16);
		for (int y = 0; y < 128; y++) {
			for (int z = 0; z < 16; z++) {
				for (int x = 0; x < 16; x++) {
					if (nbs->blocks[y][z][x].block != nullptr && nbs->blocks[y][z][x].visible) {
						nbs->blocks[y][z][x].block->Draw(x, y, z);
					}
				}
			}
		}
		glPopMatrix();
		nbs = nbs->r;
		glPushMatrix();
		glTranslatef(0, 0, 16);
		for (int y = 0; y < 128; y++) {
			for (int z = 0; z < 16; z++) {
				for (int x = 0; x < 16; x++) {
					if (nbs->blocks[y][z][x].block != nullptr && nbs->blocks[y][z][x].visible) {
						nbs->blocks[y][z][x].block->Draw(x, y, z);
					}
				}
			}
		}
		glPopMatrix();
		nbs = nbs->r;
		glPushMatrix();
		glTranslatef(16, 0, 16);
		for (int y = 0; y < 128; y++) {
			for (int z = 0; z < 16; z++) {
				for (int x = 0; x < 16; x++) {
					if (nbs->blocks[y][z][x].block != nullptr && nbs->blocks[y][z][x].visible) {
						nbs->blocks[y][z][x].block->Draw(x, y, z);
					}
				}
			}
		}
		glPopMatrix();
		nbs = nbs->f;
		glPushMatrix();
		glTranslatef(16, 0, 0);
		for (int y = 0; y < 128; y++) {
			for (int z = 0; z < 16; z++) {
				for (int x = 0; x < 16; x++) {
					if (nbs->blocks[y][z][x].block != nullptr && nbs->blocks[y][z][x].visible) {
						nbs->blocks[y][z][x].block->Draw(x, y, z);
					}
				}
			}
		}
		glPopMatrix();
		nbs = nbs->f;
		glPushMatrix();
		glTranslatef(16, 0, -16);
		for (int y = 0; y < 128; y++) {
			for (int z = 0; z < 16; z++) {
				for (int x = 0; x < 16; x++) {
					if (nbs->blocks[y][z][x].block != nullptr && nbs->blocks[y][z][x].visible) {
						nbs->blocks[y][z][x].block->Draw(x, y, z);
					}
				}
			}
		}
		glPopMatrix();
		nbs = nbs->l;
		glPushMatrix();
		glTranslatef(0, 0, -16);
		for (int y = 0; y < 128; y++) {
			for (int z = 0; z < 16; z++) {
				for (int x = 0; x < 16; x++) {
					if (nbs->blocks[y][z][x].block != nullptr && nbs->blocks[y][z][x].visible) {
						nbs->blocks[y][z][x].block->Draw(x, y, z);
					}
				}
			}
		}
		glPopMatrix();
		nbs = nbs->l;
		glPushMatrix();
		glTranslatef(-16, 0, -16);
		for (int y = 0; y < 128; y++) {
			for (int z = 0; z < 16; z++) {
				for (int x = 0; x < 16; x++) {
					if (nbs->blocks[y][z][x].block != nullptr && nbs->blocks[y][z][x].visible) {
						nbs->blocks[y][z][x].block->Draw(x, y, z);
					}
				}
			}
		}
		glPopMatrix();
		//playerSet = GetBlockSet(player::pos.x, player::pos.z);
	}

	bool BlockVisible(int x, int y, int z) {
		return !(GetBlock(x + 1, y, z)->block && GetBlock(x, y + 1, z)->block && GetBlock(x, y, z + 1)->block &&
			GetBlock(x - 1, y, z)->block && GetBlock(x, y - 1, z)->block && GetBlock(x, y, z - 1)->block);
	}

	void SetBlockSetBlockVisiable(BlockSet *blockSet) {
		for (int x = 0; x < 16; x++) {
			for (int z = 0; z < 16; z++) {
				for (int y = 1; y < 127; y++) {
					if (blockSet->blocks[y][z][x].block) {
						blockSet->blocks[y][z][x].visible = BlockVisible(blockSet->x * 16 + x, y, blockSet->z * 16 + z);
					}
				}
			}
		}
	}

	void SetBlock(BlockSet *blockSet, int x, int y, int z, Block *block) {
		if (y < 0) {
			return;
		}
		if (blockSet != nullptr) {
			blockSet->blocks[y][z][x].visible = true;
			blockSet->blocks[y][z][x].block = block;
		}
	}

	void SetBlock(int x, int y, int z, Block *block) {
		SetBlock(GetBlockSet(x, z), x % 16, y, z % 16, block);
	}

	void SetBlock(Vector3i pos, Block *block) {
		SetBlock(pos.x, pos.y, pos.z, block);
	}

	void PutBlock(Vector3i pos, Block *block) {  // 检测
		if (GetBlock(pos)->block) {
			return;
		}
		if ((int)round(player::pos.x) == pos.x && (int)round(player::pos.z) == pos.z &&
		   ((int)round(player::pos.y) == pos.y || (int)round(player::pos.y) == pos.y + 1)) {
			return;
		}
		SetBlock(pos, block);
	}

	BlockInfo *GetBlock(BlockSet *blockSet, int x, int y, int z) {
		if (y < 0) {
			return nullptr;
		}
		if (blockSet != nullptr) {
			int xx = x % 16, zz = z % 16;
			return &blockSet->blocks[y][zz][xx];
		}
		return nullptr;
	}

	BlockInfo *GetBlock(int x, int y, int z) {
		if (y < 0) {
			return nullptr;
		}
		BlockSet *blockSet = GetBlockSet(x, z);
		return GetBlock(blockSet, x, y, z);
	}

	BlockInfo *GetBlock(Vector3i pos) {
		return GetBlock(pos.x, pos.y, pos.z);
	}

	void RemoveBlock(int x, int y, int z) {
		if (y < 0) {
			return;
		}
		BlockSet *blockSet = GetBlockSet(x, z);
		if (blockSet != nullptr) {
			int xx = x % 16, zz = z % 16;
			blockSet->blocks[y][zz][xx].block = nullptr;
			int bx = blockSet->x * 16, bz = blockSet->z * 16;
			GetBlock(bx + xx + 1, y, bz + zz)->visible = BlockVisible(bx + xx + 1, y, bz + zz);
			GetBlock(bx + xx, y + 1, bz + zz)->visible = BlockVisible(bx + xx, y + 1, bz + zz);
			GetBlock(bx + xx, y, bz + zz + 1)->visible = BlockVisible(bx + xx, y, bz + zz + 1);
			GetBlock(bx + xx - 1, y, bz + zz)->visible = BlockVisible(bx + xx - 1, y, bz + zz);
			GetBlock(bx + xx, y - 1, bz + zz)->visible = BlockVisible(bx + xx, y - 1, bz + zz);
			GetBlock(bx + xx, y, bz + zz - 1)->visible = BlockVisible(bx + xx, y, bz + zz - 1);
		}
	}

	void RemoveBlock(Vector3i pos) {
		RemoveBlock(pos.x, pos.y, pos.z);
	}

	void DestoryBlock(Vector3i pos) {  // 带有粒子效果
		graphic::ParticalEffect(pos, GetBlock(pos)->block->texs, 6, 10, 5, 0.8f, 20);
		RemoveBlock(pos);
	}

	bool GetCollider(int x, int y, int z) {
		if (y < 0) {
			return false;
		}
		BlockSet *blockSet = GetBlockSet(x, z);
		if (blockSet != nullptr) {
			return blockSet->blocks[y][z % 16][x % 16].block || blockSet->blocks[y - 1][z % 16][x % 16].block;
		}
		return false;
	}
}