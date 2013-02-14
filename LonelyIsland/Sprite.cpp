#include "Sprite.h"

Sprite::Sprite()
{
	lastTime = 0;
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

void Sprite::move(double dtime)
{
}

double Sprite::getDistance(const Camera& cam)
{
	return sqrt((x-cam.Position().x)*(x-cam.Position().x) + 
		        (y-cam.Position().y)*(y-cam.Position().y) + 
				(z-cam.Position().z)*(z-cam.Position().z));
}