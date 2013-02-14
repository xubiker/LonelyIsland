#include <gl/glew.h>

#include "Bird.h"

vector<GLuint> Bird::textures;

void Bird::init()
{
	string path = "bird/", filename = ".bmp";
	for(int i = 0; i < 9; ++i) {
		Texture t(((path + (char)('0' + i)) + filename).c_str());
		t.toBird();
		textures.push_back(t.get());
	}
}

Bird::Bird() : r(50.0),	size(3.0),	speed(1/5.0)
{
	x = 0;
	y = 30.0;
	z = r;
	time = 0;
}

void Bird::move(double time)
{
	x = - r * sin(time * speed);
	z = r * cos(time * speed);
	texture = textures[(int)(time * 8) % 9];
}

void Bird::render(const Camera& cam)
{
	glUseProgram(0);

	double vx = x - cam.Position().x;
	double vy = y - cam.Position().y - 10.0;
	double vz = z - cam.Position().z;
	double nhx = - vz / sqrt(vx * vx + vz * vz);
	double nhy = 0;
	double nhz = vx / sqrt(vx * vx + vz * vz);
	double nvx = - nhz;
	double nvy = (nhz * vx - nhx * vz) / vy;
	double nvz = nhx;
	double t = sqrt(nvx * nvx + nvy * nvy + nvz * nvz);
	if (nvy < 0)
		t = - t;
	nvx /= t;
	nvy /= t;
	nvz /= t;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND );
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d (x + size * ( nhx - nvx), y + size * ( nhy - nvy), z + size * ( nhz - nvz));
		glTexCoord2f(0.0f, 1.0f);
		glVertex3d (x + size * ( nhx + nvx), y + size * ( nhy + nvy), z + size * ( nhz + nvz));
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d (x + size * (-nhx + nvx), y + size * (-nhy + nvy), z + size * (-nhz + nvz));
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d (x + size * (-nhx - nvx), y + size * (-nhy - nvy), z + size * (-nhz - nvz));
	glEnd();
	
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);
}