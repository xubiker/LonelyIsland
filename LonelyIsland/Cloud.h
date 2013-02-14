#pragma once

#include <gl/glew.h>
#include <vector>
#include <string>

#include "Sprite.h"
#include "Textures.h"

using namespace std;

class Cloud: public Sprite
{
	double size;

	const static int nTex;
	const static double x0;
	const static double y0;
	const static double z0;

	static vector<GLuint> textures;
public:
	static void init();
	Cloud();
	void render(const Camera& cam);
};