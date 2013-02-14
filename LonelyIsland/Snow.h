#pragma once

#include <gl/glew.h>
#include <gl/glut.h>
#include <gl/freeglut.h>

#include <cstdlib>
#include <vector>
#include <string>

#include "Sprite.h"
#include "Textures.h"

using namespace std;

class Snowflake:public Sprite
{
	double alpha;
	double size;

	const static double v;
	const static double x0;
	const static double y0;
	const static double z0;
	const static int nTex;

	static vector<GLuint> textures;
public:
	static void init();
	Snowflake(double time);
	void move(double t);
	void render(const Camera& cam);
};