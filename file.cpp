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

		fclose(pFile);

		// ����һ���µ������� 
		glGenTextures(1, &textureID);
		if (textureID == 0) {
			free(pixels);
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

	GLuint LoadBMPTexture(const char *fileName, const char *ofileName) {
		GLint offset, width, height, totalBytes;
		GLint ooffset, owidth, oheight;
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

		FILE *oFile;
		fopen_s(&oFile, ofileName, "rb");
		if (oFile == 0) {
			return 0;
		}

		// ��ȡͼ��ƫ��
		fseek(oFile, 0x000A, SEEK_SET);
		fread(&ooffset, 4, 1, oFile);

		// ��ȡ�ļ���ͼ��Ŀ�Ⱥ͸߶� 
		fseek(oFile, 0x0012, SEEK_SET);
		fread(&owidth, 4, 1, oFile);
		fread(&oheight, 4, 1, oFile);

		if (owidth != width || oheight != height) {
			return 0;
		}

		// ����ÿ��������ռ�ֽ����������ݴ����ݼ����������ֽ��� 
		GLint lineBytes = width * 4;
		while (lineBytes % 4 != 0) {
			lineBytes++;
		}
		totalBytes = lineBytes * height;

		// �����������ֽ��������ڴ� 
		pixels = (GLubyte *)malloc(totalBytes);
		if (pixels == 0) {
			fclose(pFile);
			fclose(oFile);
			return 0;
		}

		// ��ȡ�������� 
		fseek(pFile, offset, SEEK_SET);
		fseek(oFile, ooffset, SEEK_SET);
		unsigned int data = 0;
		for (int i = 0; i < totalBytes; i++) {
			if (fread(&data, 3, 1, oFile) <= 0) {
				free(pixels);
				fclose(pFile);
				fclose(oFile);
				return 0;
			}
			if (fread(&pixels[i], 3, 1, pFile) <= 0) {
				free(pixels);
				fclose(pFile);
				fclose(oFile);
				return 0;
			}
			i += 3;
			pixels[i] = (data ? 0 : 255);
			//pixels[i] = (mask & data) ? 255 : 0;
		}

		/*for (int i = 0; i < totalBytes; i++) {
			if (fread(&pixels[i], 1, 3, pFile) <= 0) {
				free(pixels);
				fclose(pFile);
				fclose(oFile);
				return 0;
			}
			i += 3;
			if (fread(&pixels[i], 1, 1, oFile) <= 0) {
				free(pixels);
				fclose(pFile);
				fclose(oFile);
				return 0;
			}
		}*/

		fclose(pFile);
		fclose(oFile);

		// ����һ���µ������� 
		glGenTextures(1, &textureID);
		if (textureID == 0) {
			free(pixels);
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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, pixels);
		glBindTexture(GL_TEXTURE_2D, lastTextureID);

		free(pixels);
		return textureID;
	}
}
