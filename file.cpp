#include "file.h"

namespace file {
	GLuint LoadBMPTexture(const char *fileName) {
		GLint offset, width, height, totalBytes;
		GLubyte *pixels = 0;
		GLuint lastTextureID, textureID = 0;

		// 打开文件，如果失败，返回 
		FILE *pFile;
		fopen_s(&pFile, fileName, "rb");
		if (pFile == 0) {
			return 0;
		}

		// 读取图像偏移
		fseek(pFile, 0x000A, SEEK_SET);
		fread(&offset, 4, 1, pFile);

		// 读取文件中图象的宽度和高度 
		fseek(pFile, 0x0012, SEEK_SET);
		fread(&width, 4, 1, pFile);
		fread(&height, 4, 1, pFile);

		// 计算每行像素所占字节数，并根据此数据计算总像素字节数 
		GLint lineBytes = width * 3;
		while (lineBytes % 4 != 0) {
			lineBytes++;
		}
		totalBytes = lineBytes * height;

		// 根据总像素字节数分配内存 
		pixels = (GLubyte *)malloc(totalBytes);
		if (pixels == 0) {
			fclose(pFile);
			return 0;
		}

		// 读取像素数据 
		fseek(pFile, offset, SEEK_SET);
		if (fread(pixels, totalBytes, 1, pFile) <= 0) {
			free(pixels);
			fclose(pFile);
			return 0;
		}

		// 分配一个新的纹理编号 
		glGenTextures(1, &textureID);
		if (textureID == 0) {
			free(pixels);
			fclose(pFile);
			return 0;
		}

		// 绑定新的纹理，载入纹理并设置纹理参数 
		// 在绑定前，先获得原来绑定的纹理编号，以便在最后进行恢复
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
