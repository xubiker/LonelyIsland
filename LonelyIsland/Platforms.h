#pragma once

#include <GL/glut.h>
#include <GL/freeglut.h>

#include <iostream>
#include "CVector3.h"

class Platform {
private:
	CVector3 pos;
	double h, w;
	double vert_speed;
	double min_h, max_h;
	int direction;
	double lastTime;
	GLuint MakePlatform(double x, double y, double z);
	GLuint MakeCube(double x, double y, double z);
public:
	Platform (CVector3 pos, double min_h = 1.5, double max_h = 8, double speed = 50);
	void nextPos(double dtime);
	void Draw ();
};
