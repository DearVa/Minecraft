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

				if (vy > 0 && (GetBlock(pos.x + 0.31, ceil(pos.y - 0.2), pos.z + 0.5)->block || GetBlock(pos.x + 0.5, ceil(pos.y - 0.2), pos.z + 0.31)->block ||
					GetBlock(pos.x + 0.69f, ceil(pos.y - 0.2f), pos.z + 0.5f)->block || GetBlock(pos.x + 0.5, ceil(pos.y - 0.2f), pos.z + 0.69f)->block)) {
					vy = 0;
					pos.y = ceil(pos.y - 0.8f);  // 头顶碰撞
				}

				grounded = (GetBlock(pos.x + 0.31, ceil(pos.y - 2.0), pos.z + 0.5)->block || GetBlock(pos.x + 0.5, ceil(pos.y - 2.0), pos.z + 0.31)->block ||
					GetBlock(pos.x + 0.69, ceil(pos.y - 2.0), pos.z + 0.5)->block || GetBlock(pos.x + 0.5, ceil(pos.y - 2.0), pos.z + 0.69)->block);
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

			// 模拟粒子效果
			graphic::ParticalNode *last = graphic::particals;
			graphic::ParticalNode *node = last->next;
			while (node != nullptr) {
				graphic::Partical *p = node->partical;
				p->vel.y -= p->g * dt;
				p->pos.x += p->vel.x * dt;
				p->pos.y += p->vel.y * dt;
				p->pos.z += p->vel.z * dt;
				p->timeLeft -= dt;
				if (p->timeLeft <= 0) {
					if (node->next) {
						last->next = node->next;
					} else {
						last->next = nullptr;
						graphic::particalsEnd = last;
					}
					delete node;
				} else {
					last = node;
				}
				node = last->next;
			}

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
				Block *b = GetBlock(vs[index])->block;  // v1是起始，vs是插值，x,y,z是终点
				if (b == nullptr) {  // 插值方块没有碰撞到
					v1 = vs[index];
					b = GetBlock(x, y, z)->block;
					if (b == nullptr) {  // 终点也没有碰撞到
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
				Block *b = GetBlock(x, y, z)->block;
				if (b == nullptr) {  // 没有碰撞到
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
				Block *b = GetBlock(vs[index])->block;  // v1是起始，vs是插值，x,y,z是终点
				if (b == nullptr) {  // 差值方块没有碰撞到
					v1 = vs[index];
					b = GetBlock(x, y, z)->block;
					if (b == nullptr) {  // 终点也没有碰撞到
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