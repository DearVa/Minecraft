#include "Graphic.h"

#define PI 3.14159

namespace graphic {
	GLfloat r = 0;
	ParticalNode *particals, *particalsEnd;
	MotionObjNode *motionObjs, *motionObjsEnd;

	void ParticalEffect(Vector3i pos, GLuint *texs, int texsSize, int count, float maxVel, float lastTime, float g, float drag, bool collision) {
		for (int i = 0; i < count; i++) {
			ParticalNode *node = new ParticalNode;
			Partical *p = new Partical;
			p->pos = pos;
			p->tex = texs[rand() % texsSize];
			p->texOffset.x = (rand() % 8) / 8.0f;
			p->texOffset.y = (rand() % 8) / 8.0f;
			p->vel.x = maxVel / (rand() % 100 - 50);
			p->vel.y = maxVel / (rand() % 100 - 50);
			p->vel.z = maxVel / (rand() % 100 - 50);
			p->size = (rand() % 100) / 200.0f + 0.1f;
			p->timeLeft = lastTime;
			p->g = g;
			p->drag = drag;
			p->collision = collision;
			node->partical = p;
			node->next = nullptr;
			particalsEnd->next = node;
			particalsEnd = node;
		}
	}

	void ParticalEffect(Vector3i pos, GLuint tex, int count, float maxVel, float lastTime, float g, float drag, bool collision) {
		for (int i = 0; i < count; i++) {
			ParticalNode *node = new ParticalNode;
			Partical *p = new Partical;
			p->pos = pos;
			p->tex = tex;
			p->texOffset.x = (rand() % 8) / 8.0f;
			p->texOffset.y = (rand() % 8) / 8.0f;
			p->vel.x = maxVel / (rand() % 100 - 50);
			p->vel.y = maxVel / (rand() % 100 - 50);
			p->vel.z = maxVel / (rand() % 100 - 50);
			p->size = (rand() % 100) / 200.0f + 0.1f;
			p->timeLeft = lastTime;
			p->g = g;
			p->drag = drag;
			p->collision = collision;
			node->partical = p;
			node->next = nullptr;
			particalsEnd->next = node;
			particalsEnd = node;
		}
	}

	void Summon(MotionObj *obj) {
		MotionObjNode *node = new MotionObjNode;
		node->motionObj = obj;
		node->next = nullptr;
		motionObjsEnd->next = node;
		motionObjsEnd = node;
	}

	void FireworkEffect(Vector3i pos, GLuint tex, float lastTime, float g, float drag) {
		for (int t = 0; t < 360; t += 10) {
			for (int n = 0; n < 10; n++) {
				ParticalNode *node = new ParticalNode;
				Partical *p = new Partical;
				p->pos = pos;
				p->tex = tex;
				p->texOffset.x = (rand() % 8) / 8.0f;
				p->texOffset.y = (rand() % 8) / 8.0f;
				p->vel.x = 8.0 * n * sin(PI * t / 180.0) + 5 + (rand() % 1000) / 400.0;
				p->vel.y = 0;
				p->vel.z = 8.0 * n * cos(PI * t / 180.0) + 5 + (rand() % 1000) / 400.0;
				p->size = (rand() % 100) / 200.0f + 0.1f;
				p->timeLeft = lastTime;
				p->g = g;
				p->drag = drag;
				node->partical = p;
				node->next = nullptr;
				particalsEnd->next = node;
				particalsEnd = node;
			}
		}
	}

	void Display() {
		POINT pos;
		GetCursorPos(&pos);
		player::MouseMove(pos.x, pos.y);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();  

		glRotatef(player::rot.y, 1, 0, 0);  // 摄像机变换
		glRotatef(player::rot.x, 0, 1, 0);
		glTranslated(wpx - player::pos.x, -player::pos.y, player::pos.z - wpz);

		// 绘制世界
		glEnable(GL_TEXTURE_2D);
		world::DrawWorld();

		// 绘制实体
		glEnable(GL_BLEND);
		MotionObjNode *mNode = motionObjs->next;
		while (mNode != nullptr) {
			MotionObj *o = mNode->motionObj;
			glPushMatrix();
			glTranslated(o->pos.x - wpx, o->pos.y, wpz - o->pos.z);
			glRotatef(-player::rot.x, 0, 1, 0);
			glRotatef(-player::rot.y, 1, 0, 0);
			glBindTexture(GL_TEXTURE_2D, o->tex);
			glBegin(GL_QUADS);
			float size = 0.1f * o->size;
			glTexCoord2f(0, 0);
			glVertex3f(-size, -size, 0);
			glTexCoord2f(1, 0);
			glVertex3f(size, -size, 0);
			glTexCoord2f(1, 1);
			glVertex3f(size, size, 0);
			glTexCoord2f(0, 1);
			glVertex3f(-size, size, 0);
			glEnd();
			glPopMatrix();
			mNode = mNode->next;
		}
		glDisable(GL_BLEND);

		// 绘制粒子效果
		ParticalNode *pNode = particals->next;
		while (pNode != nullptr) {
			Partical *p = pNode->partical;
			glPushMatrix();
			glTranslated(p->pos.x - wpx, p->pos.y, wpz - p->pos.z);
			glRotatef(-player::rot.x, 0, 1, 0);
			glRotatef(-player::rot.y, 1, 0, 0);
			glBindTexture(GL_TEXTURE_2D, p->tex);
			glBegin(GL_QUADS);
			float size = 0.1f * p->size;
			glTexCoord2f(p->texOffset.x, p->texOffset.y);
			glVertex3f(-size, -size, 0);
			glTexCoord2f(p->texOffset.x + 0.125f, p->texOffset.y);
			glVertex3f(size, -size, 0);
			glTexCoord2f(p->texOffset.x + 0.125f, p->texOffset.y + 0.125f);
			glVertex3f(size, size, 0);
			glTexCoord2f(p->texOffset.x, p->texOffset.y + 0.125f);
			glVertex3f(-size, size, 0);
			glEnd();
			glPopMatrix();
			pNode = pNode->next;
		}

		glDisable(GL_TEXTURE_2D);

		// 绘制玩家选择
		if (player::hit) {
			glPushMatrix();
			Vector3i pos = player::hit->pos;
			glTranslated(pos.x - wpx, pos.y, wpz - pos.z);
			glColor3f(0, 0, 0);
			glLineWidth(3);
			glBegin(GL_LINE_LOOP);
			Vector3i face = player::hit->face;
			if (face.x == 1) {
				glVertex3f(0.5001f, -0.5f, -0.5f);
				glVertex3f(0.5001f, +0.5f, -0.5f);
				glVertex3f(0.5001f, +0.5f, +0.5f);
				glVertex3f(0.5001f, -0.5f, +0.5f);
			} else if (face.x == -1) {
				glVertex3f(-0.5001f, -0.5f, -0.5f);
				glVertex3f(-0.5001f, +0.5f, -0.5f);
				glVertex3f(-0.5001f, +0.5f, +0.5f);
				glVertex3f(-0.5001f, -0.5f, +0.5f);
			} else if (face.y == 1) {
				glVertex3f(-0.5f, +0.5001f, -0.5f);
				glVertex3f(+0.5f, +0.5001f, -0.5f);
				glVertex3f(+0.5f, +0.5001f, +0.5f);
				glVertex3f(-0.5f, +0.5001f, +0.5f);
			} else if (face.y == -1) {
				glVertex3f(-0.5f, -0.5001f, -0.5f);
				glVertex3f(+0.5f, -0.5001f, -0.5f);
				glVertex3f(+0.5f, -0.5001f, +0.5f);
				glVertex3f(-0.5f, -0.5001f, +0.5f);
			} else if (face.z == 1) {
				glVertex3f(-0.5f, -0.5f, -0.5001f);
				glVertex3f(+0.5f, -0.5f, -0.5001f);
				glVertex3f(+0.5f, +0.5f, -0.5001f);
				glVertex3f(-0.5f, +0.5f, -0.5001f);
			} else if (face.z == -1) {
				glVertex3f(-0.5f, -0.5f, +0.5001f);
				glVertex3f(+0.5f, -0.5f, +0.5001f);
				glVertex3f(+0.5f, +0.5f, +0.5001f);
				glVertex3f(-0.5f, +0.5f, +0.5001f);
			}
			glEnd();
			glPopMatrix();
		}

		glutSwapBuffers();
	}

	void Initial(int width, int height) {
		glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
		glutInitWindowSize(1920, 1080);
		glutCreateWindow("Minecraft");
		glutFullScreen();
		glutSetCursor(GLUT_CURSOR_NONE);
		glutDisplayFunc(Display);
		glutIdleFunc(Display);
		glutKeyboardFunc(player::KeyBoard);
		glutKeyboardUpFunc(player::KeyBoardUp);
		//glutPassiveMotionFunc(player::MouseMove);
		glutMouseFunc(player::Mouse);

		glShadeModel(GL_SMOOTH);
		glClearColor(135 / 255.0f, 206 / 255.0f, 235 / 255.0f, 0);
		glClearDepth(1.0f);

		GLfloat lightSpecular[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat lightPosition[] = { 100000000, 128, 100000000, 0.0 };
		glMaterialfv(GL_FRONT, GL_SPECULAR, lightSpecular);
		glMaterialf(GL_FRONT, GL_SHININESS, 100.0);
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
		//glEnable(GL_ALPHA_TEST);
		//glAlphaFunc(GL_GREATER, 0.9);
		//glDepthMask(GL_FALSE);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_COLOR_MATERIAL);

		GLfloat whiteLight[] = { 0.05f, 0.05f, 0.05f, 1.0f };
		GLfloat sourceLight[] = { 0.25f, 0.25f, 0.25f, 1.0f };
		GLfloat lightPos[] = { -10.f, 5.0f, 5.0f, 1.0f };

		//glFrontFace(GL_CCW);        // Counter clock-wise polygons face out
		//glEnable(GL_CULL_FACE);        // Do not calculate inside of jet

		//// Enable lighting
		//glEnable(GL_LIGHTING);

		//// Setup and enable light 0
		////设置光照模式：缺省的环境照明
		//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteLight);

		////设置环境光
		//glLightfv(GL_LIGHT0, GL_AMBIENT, sourceLight);

		////设置散射光
		//glLightfv(GL_LIGHT0, GL_DIFFUSE, sourceLight);

		////设置光照位置
		//glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

		////启用此光照
		//glEnable(GL_LIGHT0);

		////启用颜色追踪
		//glEnable(GL_COLOR_MATERIAL);

		////多边形正面材料环境和散射颜色 追踪glcolor
		//glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

		////进行了一些参数设置
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//放大过滤器：线性过滤
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//缩小过滤器：线性过滤

		////指定环境模式为：纹理单位颜色与几何图形颜色相乘。
		//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glHint(GL_POINT_SMOOTH, GL_NICEST);

		srand((int)time(0));
		particals = new ParticalNode;
		particals->partical = nullptr;
		particals->next = nullptr;
		particalsEnd = particals;

		motionObjs = new MotionObjNode;
		motionObjs->motionObj = nullptr;
		motionObjs->next = nullptr;
		motionObjsEnd = motionObjs;

		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		gluPerspective(60.0f, (GLfloat)width / height, 0.1f, 110.0f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	void MotionObj::OnDeath() {
		if (type == 0) {
			FireworkEffect(pos, blockMgr::PINK, 2, 5, 0.3);
		}
	}
}