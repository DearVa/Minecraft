#include "physics.h"
#include "player.h"
#include "graphic.h"

namespace physics {
	using namespace player;

	float val = 3.1415926535 / 180;
	Vector3i *vs = new Vector3i[6];
	RayCastHit rayCastHit;

	void Initial(long ms) {
		std::thread phyThread = std::thread(PhysicsLoop, ms);
		phyThread.detach();
	}

	void PhysicsLoop(long ms) {
		double dt = ms / 1000.0;
		while (true) {
			clock_t startTime = clock();
			if (w) {
				vf = speed;
			} else if (s) {
				vf = -speed;
			} else {
				if (grounded) {
					vf = 0;
				} else {
					vf -= vf * 5 * dt;
				}
			}
			if (a) {
				vr = -speed;
			} else if (d) {
				vr = speed;
			} else {
				if (grounded) {
					vr = 0;
				} else {
					vr -= vr * 5 * dt;
				}
			}

			vx = vf * sin(rot.x * val) + vr * sin((rot.x + 90) * val);
			vz = vf * cos(rot.x * val) + vr * cos((rot.x + 90) * val);

			if (pos.y >= 0) {
				int posx = round(pos.x), posz = round(pos.z);

				bool pf = pos.z > posz + 0.3;
				bool pr = pos.x > posx + 0.3;
				bool pb = pos.z < posz - 0.3;
				bool pl = pos.x < posx - 0.3;
				bool lf = GetCollider(posx - 1, pos.y, posz + 1);
				bool f = GetCollider(posx, pos.y, posz + 1);
				bool rf = GetCollider(posx + 1, pos.y, posz + 1);
				bool l = GetCollider(posx - 1, pos.y, posz);
				bool r = GetCollider(posx + 1, pos.y, posz);
				bool lb = GetCollider(posx - 1, pos.y, posz - 1);
				bool b = GetCollider(posx, pos.y, posz - 1);
				bool rb = GetCollider(posx + 1, pos.y, posz - 1);

				if (vz > 0 && pf && (f || (pl && lf && !l && !lf) || (pr && rf && !r && !rf))) {
					vz = 0;
					pos.z = posz + 0.30001;
				} else if (vz < 0 && pb && (b || (pl && lb && !l && !lb) || (pr && rb && !r && !rb))) {
					vz = 0;
					pos.z = posz - 0.30001;
				}

				if (vx > 0 && pr && (r || (pf && rf && !f) || (pb && rb && !b))) {
					vx = 0;
					pos.x = posx + 0.30001;
				} else if (vx < 0 && pl && (l || (pf && lf && !f) || (pb && lb && !b))) {
					vx = 0;
					pos.x = posx - 0.30001;
				}

				BlockInfo *b1 = GetBlockInfo(pos.x + 0.31, ceil(pos.y - 0.2), pos.z + 0.5);
				BlockInfo *b2 = GetBlockInfo(pos.x + 0.5, ceil(pos.y - 0.2), pos.z + 0.31);
				BlockInfo *b3 = GetBlockInfo(pos.x + 0.69, ceil(pos.y - 0.2), pos.z + 0.5);
				BlockInfo *b4 = GetBlockInfo(pos.x + 0.5, ceil(pos.y - 0.2), pos.z + 0.69);

				if (vy > 0 && (b1 && b1->block) || (b2 && b2->block) || (b3 && b3->block) || (b4 && b4->block)) {
					vy = 0;
					//pos.y = ceil(pos.y - 0.8f);  // 头顶碰撞
				}

				b1 = GetBlockInfo(pos.x + 0.31, ceil(pos.y - 2.0), pos.z + 0.5);
				b2 = GetBlockInfo(pos.x + 0.5, ceil(pos.y - 2.0), pos.z + 0.31);
				b3 = GetBlockInfo(pos.x + 0.69, ceil(pos.y - 2.0), pos.z + 0.5);
				b4 = GetBlockInfo(pos.x + 0.5, ceil(pos.y - 2.0), pos.z + 0.69);

				grounded = (b1 && b1->block) || (b2 && b2->block) || (b3 && b3->block) || (b4 && b4->block);
			}
			
			if (grounded) {  // 模拟重力
				if (vy < 0) {
					vy = 0;
					pos.y = ceil(pos.y);
				}
			} else {
				vy -= 30.0f * dt;
			}

			pos.x += vx * dt;
			pos.y += vy * dt;
			pos.z += vz * dt;

			// 模拟实体
			graphic::MotionObjNode *mLast = graphic::motionObjs;
			graphic::MotionObjNode *mNode = mLast->next;
			while (mNode != nullptr) {
				graphic::MotionObj *o = mNode->motionObj;
				o->pos.x += o->vel.x * dt;
				o->pos.y += o->vel.y * dt;
				o->pos.z += o->vel.z * dt;
				o->timeLeft -= dt;
				if (o->timeLeft <= 0) {
					if (mNode->next) {
						mLast->next = mNode->next;
					} else {
						mLast->next = nullptr;
						graphic::motionObjsEnd = mLast;
					}
					o->OnDeath();
					delete mNode;
				} else {
					mLast = mNode;
				}
				mNode = mLast->next;
			}

			// 模拟粒子效果
			graphic::ParticalNode *pLast = graphic::particals;
			graphic::ParticalNode *pNode = pLast->next;
			while (pNode != nullptr) {
				graphic::Partical *p = pNode->partical;
				BlockInfo *down = GetBlockInfo(p->pos.x + 0.5, ceil(p->pos.y - 0.5), p->pos.z + 0.5);
				if (p->collision) {
					if (down && down->block) {
						p->vel.y = -0.8 * p->vel.y;
					}
				}
				if (p->size > p->timeLeft) {
					p->size = p->timeLeft;
				}
				p->vel.y -= p->g * dt;
				p->pos.x += p->vel.x * dt;
				p->pos.y += p->vel.y * dt;
				p->pos.z += p->vel.z * dt;
				p->vel -= p->vel * p->drag;
				p->timeLeft -= dt;
				if (p->timeLeft <= 0) {
					if (pNode->next) {
						pLast->next = pNode->next;
					} else {
						pLast->next = nullptr;
						graphic::particalsEnd = pLast;
					}
					delete pNode;
				} else {
					pLast = pNode;
				}
				pNode = pLast->next;
			}

			//cout << pos.x << " " << pos.z << "  " << px << " " << pz << endl;

			long t = ms + startTime - clock();
			if (t <= 0) {
				continue;
			}
			Sleep(t);
		}
	}

	RayCastHit *Raycast(Vector3 origin, Vector2 direction, GLfloat dis) {
		double sinrx = sin(direction.x * val);
		double cosrx = cos(direction.x * val);
		double sinry = sin(-direction.y * val);
		double cosry = cos(-direction.y * val);
		Vector3i v1 = Vector3i(round(origin.x), round(origin.y), round(origin.z));
		for (int t = 1; t < dis; t++) {
			int x = round(origin.x + t * sinrx * cosry);
			int y = round(origin.y + t * sinry);
			if (y > 127) {
				return nullptr;
			}
			int z = round(origin.z + t * cosrx * cosry);
			if (x != v1.x && y != v1.y && z != v1.z) { // 跳过了两个方块
				vs[0] = Vector3i(x, v1.y, v1.z);
				vs[1] = Vector3i(v1.x, y, v1.z);
				vs[2] = Vector3i(v1.x, v1.y, z);
				vs[3] = Vector3i(v1.x, y, z);
				vs[4] = Vector3i(x, v1.y, z);
				vs[5] = Vector3i(x, y, v1.z);
				Vector3 mid = Vector3(origin.x + (t - 0.5) * sinrx * cosry, origin.y + (t - 0.5) * sinry, origin.z + (t - 0.5) * cosrx * cosry);
				float dMin = 10;  // 选一个较大的数
				int index;
				for (int i = 0; i < 6; i++) {
					float d = (mid - vs[i]).length();
					if (d < dMin) {
						dMin = d;
						index = i;
					}
				}
				BlockInfo *b = GetBlockInfo(vs[index]);  // v1是起始，vs是插值，x,y,z是终点
				if (!(b && b->block)) {  // 插值方块没有碰撞到
					v1 = vs[index];
					b = GetBlockInfo(x, y, z);
					if (!(b && b->block)) {  // 终点也没有碰撞到
						v1 = Vector3i(x, y, z);
						continue;
					} else {
						rayCastHit.pos = Vector3i(x, y, z);
						rayCastHit.face = v1 - rayCastHit.pos;
						return &rayCastHit;
					}
				} else {
					rayCastHit.pos = vs[index];
					rayCastHit.face = v1 - rayCastHit.pos;
					return &rayCastHit;
				}
			} else if ((x != v1.x && y == v1.y && z == v1.z) || (x == v1.x && y != v1.y && z == v1.z) || (x == v1.x && y == v1.y && z != v1.z)) {  // 没有跳过方块
				BlockInfo *b = GetBlockInfo(x, y, z);
				if (!(b && b->block)) {  // 没有碰撞到
					v1 = Vector3i(x, y, z);
					continue;
				}
				rayCastHit.pos = Vector3i(x, y, z);
				rayCastHit.face = v1 - rayCastHit.pos;
				return &rayCastHit;
			} else {  // 跳过了一个方块
				if (x != v1.x && y != v1.y) {
					vs[0] = Vector3i(x, v1.y, z);
					vs[1] = Vector3i(v1.x, y, z);
				} else if (x != v1.x && z != v1.z) {
					vs[0] = Vector3i(x, y, v1.z);
					vs[1] = Vector3i(v1.x, y, z);
				} else {
					vs[0] = Vector3i(x, y, v1.z);
					vs[1] = Vector3i(x, v1.y, z);
				}
				Vector3 mid = Vector3(origin.x + (t - 0.5) * sinrx * cosry, origin.y + (t - 0.5) * sinry, origin.z + (t - 0.5) * cosrx * cosry);
				float dMin = 10;  // 选一个较大的数
				int index;
				for (int i = 0; i < 2; i++) {
					float d = (mid - vs[i]).length();
					if (d < dMin) {
						dMin = d;
						index = i;
					}
				}
				BlockInfo *b = GetBlockInfo(vs[index]);  // v1是起始，vs是插值，x,y,z是终点
				if (!(b && b->block)) {  // 差值方块没有碰撞到
					v1 = vs[index];
					b = GetBlockInfo(x, y, z);
					if (!(b && b->block)) {  // 终点也没有碰撞到
						v1 = Vector3i(x, y, z);
						continue;
					} else {
						rayCastHit.pos = Vector3i(x, y, z);
						rayCastHit.face = v1 - rayCastHit.pos;
						return &rayCastHit;
					}
				} else {
					rayCastHit.pos = vs[index];
					rayCastHit.face = v1 - rayCastHit.pos;
					return &rayCastHit;
				}
			}
		}
		return nullptr;
	}
}