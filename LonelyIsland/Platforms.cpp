#include "Platforms.h"

Platform::Platform(CVector3 pos, double min_h, double max_h, double speed)
{
	this->pos = pos;
	this->min_h = min_h;
	this->max_h = max_h;
	this->vert_speed = 0.01 * speed;
	direction = 1;
	h = 0.5;
	w = 2.5;
	lastTime = 0;
}

void Platform::nextPos (double time)
{
	pos.y += (time - lastTime) * vert_speed * direction;
	if (pos.y <= min_h)
		direction = 1;
	else if (pos.y >= max_h)
		direction = -1;
	lastTime = time;
}

GLuint Platform::MakePlatform (double x, double y, double z)
{
	GLuint list = glGenLists(1);
	glNewList(list, GL_COMPILE);
		glColor3d(1.0, 0.0, 0.0);
		glBegin(GL_QUAD_STRIP);
		// 1-2-3-4
		glVertex3d (x + w/2, y - h/2, z + w/2);
		glVertex3d (x + w/2, y + h/2, z + w/2);
		glVertex3d (x - w/2, y - h/2, z + w/2);
		glVertex3d (x - w/2, y + h/2, z + w/2);
		// 4-3-7-8
		glVertex3d (x - w/2, y - h/2, z - w/2);
		glVertex3d (x - w/2, y + h/2, z - w/2);
		// 8-7-6-5
		glVertex3d (x + w/2, y - h/2, z - w/2);
		glVertex3d (x + w/2, y + h/2, z - w/2);
		// 5-6-2-1
		glVertex3d (x + w/2, y - h/2, z + w/2);
		glVertex3d (x + w/2, y + h/2, z + w/2);
		glEnd();
		glBegin(GL_QUADS);
		// 2-6-7-3
		glVertex3d (x + w/2, y + h/2, z + w/2);
		glVertex3d (x + w/2, y + h/2, z - w/2);
		glVertex3d (x - w/2, y + h/2, z - w/2);
		glVertex3d (x - w/2, y + h/2, z + w/2);
		// 1-4-8-5
		glVertex3d (x + w/2, y - h/2, z + w/2);
		glVertex3d (x - w/2, y - h/2, z + w/2);
		glVertex3d (x - w/2, y - h/2, z - w/2);
		glVertex3d (x + w/2, y - h/2, z - w/2);
		glEnd();
	glEndList();
	return list;
}

GLuint Platform::MakeCube (double x, double y, double z)
{
	GLuint list = glGenLists(1);
	glNewList(list, GL_COMPILE);
		glBegin(GL_QUADS);
		glColor3d(1.0, 1.0, 1.0);
		// 5-7-6-4
		glTexCoord2d(0,0);
		glVertex3d(x + w/2, y - h/2, z + w/2);
		glTexCoord2d(1,0);
		glVertex3d(x + w/2, y + h/2, z + w/2);
		glTexCoord2d(1,1);
		glVertex3d(x - w/2, y + h/2, z + w/2);
		glTexCoord2d(0,1);
		glVertex3d(x - w/2, y - h/2, z + w/2);
		// 4-6-2-0
		glTexCoord2d(0,0);
		glVertex3d(x - w/2, y - h/2, z + w/2);
		glTexCoord2d(1,0);
		glVertex3d(x - w/2, y + h/2, z + w/2);
		glTexCoord2d(1,1);
		glVertex3d(x - w/2, y + h/2, z - w/2);
		glTexCoord2d(0,1);
		glVertex3d(x - w/2, y - h/2, z - w/2);
		// 0-2-3-1
		glTexCoord2d(0,0);
		glVertex3d(x - w/2, y - h/2, z - w/2);
		glTexCoord2d(1,0);
		glVertex3d(x - w/2, y + h/2, z - w/2);
		glTexCoord2d(1,1);
		glVertex3d(x + w/2, y + h/2, z - w/2);
		glTexCoord2d(0,1);
		glVertex3d(x + w/2, y - h/2, z - w/2);
		// 1-3-7-5
		glTexCoord2d(0,0);
		glVertex3d(x + w/2, y - h/2, z - w/2);
		glTexCoord2d(1,0);
		glVertex3d(x + w/2, y + h/2, z - w/2);
		glTexCoord2d(1,1);
		glVertex3d(x + w/2, y + h/2, z + w/2);
		glTexCoord2d(0,1);
		glVertex3d(x + w/2, y - h/2, z + w/2);
		// 7-3-2-6
		glTexCoord2d(0,0);
		glVertex3d(x + w/2, y + h/2, z + w/2);
		glTexCoord2d(1,0);
		glVertex3d(x + w/2, y + h/2, z - w/2);
		glTexCoord2d(1,1);
		glVertex3d(x - w/2, y + h/2, z - w/2);
		glTexCoord2d(0,1);
		glVertex3d(x - w/2, y + h/2, z + w/2);
		// 5-4-0-1
		glTexCoord2d(0,0);
		glVertex3d(x + w/2, y - h/2, z + w/2);
		glTexCoord2d(1,0);
		glVertex3d(x - w/2, y - h/2, z + w/2);
		glTexCoord2d(1,1);
		glVertex3d(x - w/2, y - h/2, z - w/2);
		glTexCoord2d(0,1);
		glVertex3d(x + w/2, y - h/2, z - w/2);
		glEnd();
	glEndList();
	return 1;
}


void Platform::Draw ()
{
	glPushMatrix();
	GLuint list = MakeCube(pos.x, pos.y, pos.z);
	glCallList(list);
	glDeleteLists(list, 1);
	glPopMatrix();
}