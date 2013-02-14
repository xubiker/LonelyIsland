#include <gl/glew.h>

#include "Textures.h"

Texture::Texture(const char* path)
{	
	if((texBits = LoadTrueColorBMPFile(path, &width, &height)) == NULL)
		throw "Cannot open texture file";
}


void Texture::toCloud()
{
	glGenTextures(1,&id);	
	unsigned char *texBits2 = new unsigned char[width * height * 4];
	int t0 = rand() % 20;
	for (int i = 0; i < width; ++i)
		for (int j = 0; j < height; ++j) {
			texBits2[i * height * 4 + j * 4 + 3] = texBits[i * height * 3 + j * 3];
			int tcolor = t0 + rand() % 30;
			texBits2[i * height * 4 + j * 4] = 255 - tcolor;
			texBits2[i * height * 4 + j * 4 + 1] = 255 - tcolor;
			texBits2[i * height * 4 + j * 4 + 2] = 255 - tcolor / 2.0;
		}
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texBits2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	delete[] texBits;
	delete[] texBits2;
}

void Texture::toBird()
{
	glGenTextures(1, &id);
	
	unsigned char *texBits2 = new unsigned char[width * height * 4];
	for (int i = 0; i < width; ++i)
		for (int j = 0; j < height; ++j) {
			texBits2[i * height * 4 + j * 4]     = texBits[i * height * 3 + j * 3];
			texBits2[i * height * 4 + j * 4 + 1] = texBits[i * height * 3 + j * 3 + 1];
			texBits2[i * height * 4 + j * 4 + 2] = texBits[i * height * 3 + j * 3 + 2];
			if (texBits2[i * height * 4 + j * 4] > 200 &&
				texBits2[i * height * 4 + j * 4 + 1] > 200 &&
				texBits2[i * height * 4 + j * 4 + 2] > 200)
			{
				texBits2[i * height * 4 + j * 4 + 3] = 0;
			}
			else
			{
				texBits2[i * height * 4 + j * 4 + 3] = 255;
			}
		}
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texBits2);
	// Устанавливаем параметры текстуры
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	delete[] texBits;
	delete[] texBits2;
}

GLuint Texture::get()
{
	return id;
}