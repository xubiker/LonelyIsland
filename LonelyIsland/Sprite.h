#pragma once

#include "Camera.h"
#include <gl/freeglut.h>

class Sprite
{
protected:
	double x,y,z;
	GLuint texture;
	double lastTime;

public:
	Sprite();
	virtual void move(double dtime);
	virtual void render(const Camera& cam) = 0;
	double getDistance(const Camera& cam);
};