#include "physics.h"
#include "player.h"
#include "graphic.h"

namespace physics {
	using namespace player;

	float val = 3.1415926535 / 180;
	Vector3i *vs = new Vector3i[6];
	RayCastHit rayCastHit;

	void Initial(double ms) {
		std::thread phyThread = std::thread(PhysicsLoop, ms);
		phyThread.detach();
	}

	void PhysicsLoop(double ms) {
		double dt = ms / 1000;
		while (true) {
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

				/*cout << (lf ? "O" : " ") << (f ? "O" : " ") << (rf ? "O" : " ") << endl;
				cout << (l ? "O" : " ") << " " << (r ? "O" : " ") << endl;
				cout << (lb ? "O" : " ") << (b ? "O" : " ") << (rb ? "O" : " ") << endl;
				cout << pf << " " << pr << " " << pb << " " << pl << endl;
				cout << posx << " " << pos.y << " " << posz << endl << endl;*/

				if (vz > 0 && pf && (f || (pl && lf && !l) || (pr && rf && !r))) {
					vz = 0;
					pos.z = posz + 0.30001;
				} else if (vz < 0 && pb && (b || (pl && lb && !l) || (pr && rb && &r))) {
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

				if (vy > 0 && (GetBlock(posx + 0.31, ceil(pos.y - 0.2), posz + 0.5) || GetBlock(posx + 0.5, ceil(pos.y - 0.2), posz + 0.31) ||
					GetBlock(posx + 0.69f, ceil(pos.y - 0.2f), posz + 0.5f) || GetBlock(posx + 0.5, ceil(pos.y - 0.2f), posz + 0.69f))) {
					vy = 0;
					pos.y = ceil(pos.y - 0.8f);  // 头顶碰撞
				}

				grounded = (GetBlock(posx + 0.31, ceil(pos.y - 2.0), posz + 0.5) || GetBlock(posx + 0.5, ceil(pos.y - 2.0), posz + 0.31) ||
					GetBlock(posx + 0.69, ceil(pos.y - 2.0), posz + 0.5) || GetBlock(posx + 0.5, ceil(pos.y - 2.0), posz + 0.69));
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
			vector<graphic::Partical>::iterator it = graphic::particals.begin();
			while (it != graphic::particals.end()) {
				(*it).vel.y -= (*it).g * dt;
				(*it).pos.x += (*it).vel.x * dt;
				(*it).pos.y += (*it).vel.y * dt;
				(*it).pos.z += (*it).vel.z * dt;
				(*it).timeLeft -= dt;
				it++;
				//if ((*it).timeLeft <= 0) {
				//	graphic::particals.erase(it);
				//	it--;
				//}
			}
			Sleep(ms);
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
				Block *b = GetBlock(vs[index]);  // v1是起始，vs是插值，x,y,z是终点
				if (b == nullptr) {  // 差值方块没有碰撞到
					v1 = vs[index];
					b = GetBlock(x, y, z);
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
				Block *b = GetBlock(x, y, z);
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
				Block *b = GetBlock(vs[index]);  // v1是起始，vs是插值，x,y,z是终点
				if (b == nullptr) {  // 差值方块没有碰撞到
					v1 = vs[index];
					b = GetBlock(x, y, z);
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