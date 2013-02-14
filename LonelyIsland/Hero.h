#pragma once

#include "CVector3.h"
#include "camera.h"

class Hero {
	private:
		CVector3 coords;
		Camera* cam;
		GLfloat ctlpoints[12][12][3];
		GLfloat ctlpointsHead[4][4][3];
	public:
		Hero();
		void AttachCamera(Camera* _cam);
		void Move(float speed);
		void MoveUp(float speed);
		void Strafe (float speed);
		void Draw(double curTime);
		void CalcCircuit(int k,double y,double z, double alpha, double r);
};