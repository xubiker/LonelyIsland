#pragma once

#include <string>
#include <vector>

#include "Sprite.h"
#include "Textures.h"

using namespace std;

class Bird: public Sprite
{
	const double size;
	const double r;
	const double speed;
	double time;

	static vector<GLuint> textures;
public:
	static void init();
	Bird();
	void move(double dtime);
	void render(const Camera& cam);
};