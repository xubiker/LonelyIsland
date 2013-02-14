#pragma once

#include <gl/glew.h>
#include <gl/glut.h>

#include <iostream>
#include <fstream>

#include "BmpLoad.h"

using namespace std;


class Shader {
	public:
		GLuint texID[18];
		GLenum program;
		void InitTexture ();
		void LoadTrueColorTexture (const char *path, GLenum program, int idx, const char *sampler);
		void PrintInfoLog (const char *prefix, GLenum obj);
		bool initShader (const char* shV, const char* shF);
		void setupTexture(GLint id, const GLchar *name, GLuint texture);
};