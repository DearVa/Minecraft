#include "file.h"

namespace file {
	GLuint LoadBMPTexture(const char *fileName) {
		GLint offset, width, height, totalBytes;
		GLubyte *pixels = 0;
		GLuint lastTextureID, textureID = 0;

		// ���ļ������ʧ�ܣ����� 
		FILE *pFile;
		fopen_s(&pFile, fileName, "rb");
		if (pFile == 0) {
			return 0;
		}

		// ��ȡͼ��ƫ��
		fseek(pFile, 0x000A, SEEK_SET);
		fread(&offset, 4, 1, pFile);

		// ��ȡ�ļ���ͼ��Ŀ�Ⱥ͸߶� 
		fseek(pFile, 0x0012, SEEK_SET);
		fread(&width, 4, 1, pFile);
		fread(&height, 4, 1, pFile);

		// ����ÿ��������ռ�ֽ����������ݴ����ݼ����������ֽ��� 
		GLint lineBytes = width * 3;
		while (lineBytes % 4 != 0) {
			lineBytes++;
		}
		totalBytes = lineBytes * height;

		// �����������ֽ��������ڴ� 
		pixels = (GLubyte *)malloc(totalBytes);
		if (pixels == 0) {
			fclose(pFile);
			return 0;
		}

		// ��ȡ�������� 
		fseek(pFile, offset, SEEK_SET);
		if (fread(pixels, totalBytes, 1, pFile) <= 0) {
			free(pixels);
			fclose(pFile);
			return 0;
		}

		// ����һ���µ������� 
		glGenTextures(1, &textureID);
		if (textureID == 0) {
			free(pixels);
			fclose(pFile);
			return 0;
		}

		// ���µ������������������������ 
		// �ڰ�ǰ���Ȼ��ԭ���󶨵������ţ��Ա��������лָ�
		GLint temp;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &temp);
		lastTextureID = (GLuint)temp;
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
		glBindTexture(GL_TEXTURE_2D, lastTextureID);

		free(pixels);
		return textureID;
	}
}
