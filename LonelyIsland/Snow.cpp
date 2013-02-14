#include "Snow.h"

const double Snowflake::v = 1;
const double Snowflake::x0 = 50;
const double Snowflake::y0 = 30;
const double Snowflake::z0 = 0;
const int Snowflake::nTex = 1;
vector<GLuint> Snowflake::textures;

void Snowflake::init()
{
	string path = "snow/", fileExt = ".bmp";
	for (int i = 0; i < nTex; ++i)
	{
		Texture t(((path + (char)('0' + i)) + fileExt).c_str());
		t.toCloud();
		textures.push_back(t.get());
	}
}

Snowflake::Snowflake(double time)
{
	x = x0 + rand() % 10 - 5;
	z = z0 + rand() % 30 - 15;
	y = y0 + rand() % 4 - 2;
	alpha = (rand() % 300) / 100.0;
	texture = textures[rand() % nTex];
	lastTime = time;
	size = (rand() % 1000) / 2000.0 + 0.2;
}

void Snowflake::move(double time)
{
	double t = time - lastTime;
	y = y - t * v;
	x = x + (rand() % 100) / 500.0 * t * v;
	z = z + (rand() % 100) / 500.0 * t * v;
	alpha += 0.2 * t;
	if (y < 0.0)
		this->Snowflake::Snowflake(time);
	lastTime = time;
}

void Snowflake::render(const Camera& cam)
{
	glUseProgram(0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(x + size * cos(alpha), y + size, z + size * sin(alpha));
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(x - size * cos(alpha), y + size, z - size * sin(alpha));
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(x - size * cos(alpha), y - size, z - size * sin(alpha));
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(x + size * cos(alpha), y - size, z + size * sin(alpha));
	glEnd();

	glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);
}