#include "Cloud.h"

const int Cloud::nTex = 3;
const double Cloud::x0 = 50;
const double Cloud::y0 = 30;
const double Cloud::z0 = 0;
vector<GLuint> Cloud::textures;

void Cloud::init()
{
	string path = "cloud/", filename = ".bmp";
	for (int i = 0; i < nTex; ++i)
	{
		Texture t(((path + (char)('0' + i)) + filename).c_str());
		t.toCloud();
		textures.push_back(t.get());
	}
}

Cloud::Cloud()
{
	size = 3;
	texture = textures[rand() % nTex];
	x = x0 + ((rand() % 100) *  6.0 / 100.0 - 3.0) * ((rand() % 100) *  6.0 / 100.0 - 3.0);
	y = y0 + ((rand() % 100) *  5.0 / 100.0 - 2.5) * ((rand() % 100) *  5.0 / 100.0 - 2.5);
	z = z0 + ((rand() % 100) * 10.0 / 100.0 - 5.0) * ((rand() % 100) * 10.0 / 100.0 - 5.0);
}


void Cloud::render(const Camera& cam)
{
	double vx = x - cam.Position().x;
	double vy = y - cam.Position().y - 10.0;
	double vz = z - cam.Position().z;
	double nhx = - vz / sqrt(vx * vx + vz * vz);
	double nhy = 0;
	double nhz = vx / sqrt(vx * vx + vz * vz);
	double nvx = -nhz;
	double nvy = (nhz * vx - nhx * vz) / vy;
	double nvz = nhx;
	double t = sqrt(nvx * nvx + nvy * nvy + nvz * nvz);
	if (nvy < 0)
		t = - t;
	nvx /= t;
	nvy /= t;
	nvz /= t;

	glUseProgram(0);
	glEnable(GL_TEXTURE_2D);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
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