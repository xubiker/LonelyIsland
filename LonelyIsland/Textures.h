#pragma once

#include <gl/glut.h>
#include <gl/freeglut.h>

#include "BmpLoad.h"

class Texture
{
	unsigned char* texBits;
	int width,height;
	GLuint id;

public:
	Texture(const char* path);
	void toCloud();
	void toBird();
	GLuint get();
};