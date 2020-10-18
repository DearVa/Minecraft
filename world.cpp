#include "world.h"
#include "graphic.h"

namespace world {
	BlockSet *worldCenter, *playerSet;

	void Initial() {
		using namespace blockMgr;
		blockMgr::Initial();
		CreateWorld(grassBlock);
		SetBlock(playerSet, 6, 2, 3, stone);
		SetBlock(playerSet, 5, 1, 3, stone);
		SetBlock(playerSet, 5, 1, 4, stone);
		SetBlock(playerSet, 6, 3, 3, stone);
		SetBlock(playerSet, 6, 4, 4, stone);
		SetBlock(playerSet, 6, 3, 5, stone);
		SetBlock(playerSet, 5, 2, 5, stone);
		SetBlock(playerSet, 6, 4, 3, stone);
		SetBlock(playerSet, 6, 5, 3, stone);
		SetBlock(playerSet, 0, 2, 4, stone);
		SetBlock(playerSet, 5, 2, 7, dirt);
		SetBlock(playerSet, 6, 1, 4, dirt);
		SetBlock(playerSet, 6, 4, 8, dirt);
		SetBlock(playerSet, 5, 2, 8, dirt);
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

	void CreateWorld(Block *baseBlock) {
		worldCenter = new BlockSet;
		worldCenter->x = 6250000;
		worldCenter->z = 6250000;
		memset(worldCenter->blocks, 0, sizeof(worldCenter->blocks));
		worldCenter->f = nullptr;
		worldCenter->l = nullptr;
		worldCenter->b = nullptr;
		worldCenter->r = nullptr;
		for (int xx = 0; xx < 16; xx++) {
			for (int yy = 0; yy < 16; yy++) {
				worldCenter->blocks[0][xx][yy] = baseBlock;
			}
		}
		playerSet = worldCenter;
		BlockSet *nbsl = CreateBlockSet(worldCenter, DIRECTION::LEFT);
		for (int xx = 0; xx < 16; xx++) {
			for (int yy = 0; yy < 16; yy++) {
				nbsl->blocks[0][xx][yy] = baseBlock;
			}
		}
		BlockSet *nbs = CreateBlockSet(nbsl, DIRECTION::FRONT);
		for (int xx = 0; xx < 16; xx++) {
			for (int yy = 0; yy < 16; yy++) {
				nbs->blocks[0][xx][yy] = baseBlock;
			}
		}
		nbs = CreateBlockSet(nbsl, DIRECTION::BACK);
		for (int xx = 0; xx < 16; xx++) {
			for (int yy = 0; yy < 16; yy++) {
				nbs->blocks[0][xx][yy] = baseBlock;
			}
		}
		BlockSet *nbsr = CreateBlockSet(worldCenter, DIRECTION::RIGHT);
		for (int xx = 0; xx < 16; xx++) {
			for (int yy = 0; yy < 16; yy++) {
				nbsr->blocks[0][xx][yy] = baseBlock;
			}
		}
		nbs = CreateBlockSet(nbsr, DIRECTION::FRONT);
		for (int xx = 0; xx < 16; xx++) {
			for (int yy = 0; yy < 16; yy++) {
				nbs->blocks[0][xx][yy] = baseBlock;
			}
		}
		nbs = CreateBlockSet(nbsr, DIRECTION::BACK);
		for (int xx = 0; xx < 16; xx++) {
			for (int yy = 0; yy < 16; yy++) {
				nbs->blocks[0][xx][yy] = baseBlock;
			}
		}
		nbs = CreateBlockSet(worldCenter, DIRECTION::FRONT);
		for (int xx = 0; xx < 16; xx++) {
			for (int yy = 0; yy < 16; yy++) {
				nbs->blocks[0][xx][yy] = baseBlock;
			}
		}
		nbs = CreateBlockSet(worldCenter, DIRECTION::BACK);
		for (int xx = 0; xx < 16; xx++) {
			for (int yy = 0; yy < 16; yy++) {
				nbs->blocks[0][xx][yy] = baseBlock;
			}
		}
	}

	void DrawWorld(GLuint maxVision) {
		if (!playerSet) {
			return;
		}
		BlockSet *nbs = playerSet;
		for (int y = 0; y < 32; y++) {
			for (int z = 0; z < 16; z++) {
				for (int x = 0; x < 16; x++) {
					if (nbs->blocks[y][z][x] != nullptr) {
						nbs->blocks[y][z][x]->Draw(x, y, z);
					}
				}
			}
		}
		nbs = nbs->l;
		glPushMatrix();
		glTranslatef(-16, 0, 0);
		for (int y = 0; y < 32; y++) {
			for (int z = 0; z < 16; z++) {
				for (int x = 0; x < 16; x++) {
					if (nbs->blocks[y][z][x] != nullptr) {
						nbs->blocks[y][z][x]->Draw(x, y, z);
					}
				}
			}
		}
		glPopMatrix();
		nbs = nbs->b;
		glPushMatrix();
		glTranslatef(-16, 0, 16);
		for (int y = 0; y < 32; y++) {
			for (int z = 0; z < 16; z++) {
				for (int x = 0; x < 16; x++) {
					if (nbs->blocks[y][z][x] != nullptr) {
						nbs->blocks[y][z][x]->Draw(x, y, z);
					}
				}
			}
		}
		glPopMatrix();
		nbs = nbs->r;
		glPushMatrix();
		glTranslatef(0, 0, 16);
		for (int y = 0; y < 32; y++) {
			for (int z = 0; z < 16; z++) {
				for (int x = 0; x < 16; x++) {
					if (nbs->blocks[y][z][x] != nullptr) {
						nbs->blocks[y][z][x]->Draw(x, y, z);
					}
				}
			}
		}
		glPopMatrix();
	}

	void SetBlock(BlockSet *blockSet, int x, int y, int z, Block *block) {
		if (y < 0) {
			return;
		}
		if (blockSet != nullptr) {
			blockSet->blocks[y][z][x] = block;
		}
	}

	void SetBlock(int x, int y, int z, Block *block) {
		SetBlock(GetBlockSet(x, z), x % 16, y, z % 16, block);
	}

	void SetBlock(Vector3i pos, Block *block) {
		SetBlock(pos.x, pos.y, pos.z, block);
	}

	Block *GetBlock(int x, int y, int z) {
		if (y < 0) {
			return nullptr;
		}
		BlockSet *blockSet = GetBlockSet(x, z);
		if (blockSet != nullptr) {
			int xx = x % 16, zz = z % 16;
			return blockSet->blocks[y][zz][xx];
		}
		return nullptr;
	}

	Block *GetBlock(Vector3i pos) {
		return GetBlock(pos.x, pos.y, pos.z);
	}

	void RemoveBlock(int x, int y, int z) {
		if (y < 0) {
			return;
		}
		BlockSet *blockSet = GetBlockSet(x, z);
		if (blockSet != nullptr) {
			blockSet->blocks[y][z % 16][x % 16] = nullptr;
		}
	}

	void RemoveBlock(Vector3i pos) {
		RemoveBlock(pos.x, pos.y, pos.z);
	}

	void DestoryBlock(Vector3i pos) {  // 带有粒子效果
		graphic::ParticalEffect(pos, GetBlock(pos)->texs, 6, 10, 5, 2, 20);
		RemoveBlock(pos);
	}

	bool GetCollider(int x, int y, int z) {
		if (y < 0) {
			return false;
		}
		BlockSet *blockSet = GetBlockSet(x, z);
		if (blockSet != nullptr) {
			return blockSet->blocks[y][z % 16][x % 16] || blockSet->blocks[y - 1][z % 16][x % 16];
		}
		return false;
	}
}