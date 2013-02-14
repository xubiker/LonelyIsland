#pragma once

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include "math.h"
 

class CVector3 {
 
public:	
	CVector3() {}
 
	CVector3(float X, float Y, float Z) {
		x = X; y = Y; z = Z;
	}

	CVector3 operator+(CVector3 vVector) {
		return CVector3(vVector.x + x, vVector.y + y, vVector.z + z);
	}
 
	CVector3 operator-(CVector3 vVector) {
		return CVector3(x-vVector.x, y-vVector.y, z-vVector.z);
	}
 
	CVector3 operator*(float num) {
		return CVector3(x*num, y*num, z*num);
	}
 
	CVector3 operator/(float num) {
		return CVector3(x/num,y/num,z/num);
	}

	static float Magnitude(CVector3 vNormal)
	{
		return (float)sqrt( (vNormal.x * vNormal.x) +
				(vNormal.y * vNormal.y) +
				(vNormal.z * vNormal.z) );
	}

	static CVector3 Normalize(CVector3 vVector)
	{
		float magnitude = Magnitude(vVector);
		vVector = vVector / magnitude;
		return vVector;
	}
 
	float x, y, z;
};