#pragma once

#include <gl/glut.h>

#include <iostream>
#include <queue>

#include "include/math/math3d.h"
#include "include/math/mathgl.h"
#include "CVector3.h"

class Camera {
	public:
		Camera();
 
		CVector3 Position() const {	CVector3 res = m_vPosition; res.y *= mirror; return res;	}
		CVector3 View()		{	return m_vView;	}
		CVector3 UpVector() {	return m_vUpVector;	}
		CVector3 Strafe()	{	return m_vStrafe;}

		void PositionCamera(CVector3 position, CVector3 view, CVector3 upVector);
		void RotateView(float angle, CVector3 axis);
		void RotateAroundPoint(CVector3 vCenter, float angle, CVector3 axis);
		void RotateAroundObject(float angle, CVector3 axis);
		void SetViewByMouse();
		void SmoothMouse(float angleY, float angleZ);
		void StrafeCamera(float speed);
		void MoveCamera(float speed);
		void MoveUp(float speed);
		void ZoomCamera(float speed);
		void Update();
		void Look();
		void UpdateCursorPos();
		void SetHeroCoords(CVector3 _coords);

		CVector3 m_vView;
 		CVector3 m_vPosition;
		CVector3 m_vStrafe;
		int mirror;

	private:
		CVector3 m_vUpVector;
		CVector3 heroCoords;

		int defCursorPosX, defCursorPosY;
		float smoothAngleZ, smoothAngleY, angleZ, angleY;
};