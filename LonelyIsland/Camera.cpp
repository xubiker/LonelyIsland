// �������� ��� �����
#include "CVector3.h"
#include "Camera.h"

#include <iostream>

#define kSpeed 0.1f

 
inline CVector3 Cross(CVector3 vVector1, CVector3 vVector2)
{
	/////
	/////	���������� ���������������� ������ �� 2� ���������� ��������.
	/////   2 ����� �������������� ������� �������� ���������, �� �������� ��
	/////   � ���� �������������.
	/////
	CVector3 vNormal;
 
	// ���� � ��� ���� 2 ������� (������ ������� � ������������ ������), 
	// � ��� ���� ���������, �� ������� �� ����� ��������� ���� � 90 ��������.
	// ������� cross'a �����, �� ��� ������ ��������� � ������� ����. 
	vNormal.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));
 
	vNormal.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));
 
	vNormal.z  = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));
 
	// *�����* �� �� ������ ������ ���� �������, ����� ������ �� ����� ��������.
	// ������ ���� ������ ���, ��� �����. ������ ���������, ���� �� ����� �, �� ��
	// ����������� �������� X ���� ��������, � �� �� ����� ��� Y � Z. ��������,
	// �� ������������� �������� �� ���� ������ ����, � ������� �� ��� �� �����.
 
	// ����, ����� �� ���? ��� ����� ����� ���, ������ ������� ���������. �������� ������
	// ����� � ������ ������� - ������������ ��� ������ (0,1,0). 
	// �������� ������ ����� � ���� ����������, ��� ��� ��� ���������� ��� 
	// ���������� ����.
	return vNormal;
}

inline float Magnitude(CVector3 vNormal)
{
	return (float)sqrt( (vNormal.x * vNormal.x) +
			(vNormal.y * vNormal.y) +
			(vNormal.z * vNormal.z) );
}

inline CVector3 Normalize(CVector3 vVector)
{
	return vVector / Magnitude(vVector);
}

void Camera::SetViewByMouse()
{
	POINT mousePos;
	GetCursorPos(&mousePos);

	if (mousePos.x != defCursorPosX || mousePos.y != defCursorPosY) {
		angleY += (float)((defCursorPosX - mousePos.x)) / 300.0f;
		angleZ += (float)((defCursorPosY - mousePos.y)) / 300.0f;
		SetCursorPos(defCursorPosX, defCursorPosY);
	}

	CVector3 vAxis = Cross(m_vView - m_vPosition, m_vUpVector);
	vAxis = Normalize(vAxis);

	if (abs(angleZ) < smoothAngleZ) {
		RotateAroundObject(angleZ, vAxis);
//		RotateView(angleZ, vAxis.x, vAxis.y, vAxis.z);
		angleZ = 0;
	} else {
		if (angleZ < 0) {
//			RotateView(-smoothAngleZ, vAxis.x, vAxis.y, vAxis.z);
			RotateAroundObject(-smoothAngleZ, vAxis);
			angleZ += smoothAngleZ;
		} else if (angleZ > 0) {
//			RotateView(smoothAngleZ, vAxis.x, vAxis.y, vAxis.z);
			RotateAroundObject(smoothAngleZ, vAxis);
			angleZ -= smoothAngleZ;
		}
	}
	if (abs(angleY) < smoothAngleY) {
//		RotateView(angleY, 0, 1, 0);
		RotateAroundObject(angleY, CVector3(0, 1, 0));
		angleY = 0;
	} else {
		if (angleY < 0) {
//			RotateView(-smoothAngleY, 0, 1, 0);
			RotateAroundObject(-smoothAngleY, CVector3(0, 1, 0));
			angleY += smoothAngleY;
		} else if (angleY > 0) {
//			RotateView(smoothAngleY, 0, 1, 0);
			RotateAroundObject(smoothAngleY, CVector3(0, 1, 0));
			angleY -= smoothAngleY;
		}
	}
}

Camera::Camera()
{
	mirror = 1;
	m_vPosition = CVector3(0.0, 10.0, 0.0);		//�������������� ������ ����� ������� � ������� ����������
	m_vView = CVector3(0.0, 11.0, 0.5);  		//����. ������ �������
	m_vUpVector = CVector3(0.0, 0.0, 1.0);		//������ ����.

//	smoothAngleZ = 0.05;
	smoothAngleZ = 0.1;
//	smoothAngleY = 0.01;
	smoothAngleY = 0.1;
	angleY = 0;
	angleZ = 0;
}

void Camera::UpdateCursorPos()
{
	defCursorPosX = glutGet(GLUT_WINDOW_X)+glutGet(GLUT_WINDOW_WIDTH) / 2;
	defCursorPosY = glutGet(GLUT_WINDOW_Y)+glutGet(GLUT_WINDOW_HEIGHT) / 2;
}
 
GLvoid Camera::PositionCamera( CVector3 position, CVector3 view, CVector3 upVector)
{
	m_vPosition	= position;
	m_vView		= view;
	m_vUpVector	=  upVector;
}

void Camera::RotateAroundPoint(CVector3 vCenter, float angle, CVector3 axis)
{
    CVector3 vNewPosition;

	// ������� �����, ������ �������� ����� ��������
    CVector3 vPos = m_vPosition - vCenter;
 
    // �������� ����� � ������� ����
    float cosTheta = (float)cos(angle);
    float sinTheta = (float)sin(angle);
 
    // ������ �������� ����� ��������
    vNewPosition.x  = (cosTheta + (1 - cosTheta) * axis.x * axis.x)          * vPos.x;
    vNewPosition.x += ((1 - cosTheta) * axis.x * axis.y - axis.z * sinTheta) * vPos.y;
    vNewPosition.x += ((1 - cosTheta) * axis.x * axis.z + axis.y * sinTheta) * vPos.z;
 
    vNewPosition.y  = ((1 - cosTheta) * axis.x * axis.y + axis.z * sinTheta) * vPos.x;
    vNewPosition.y += (cosTheta + (1 - cosTheta) * axis.y * axis.y)          * vPos.y;
    vNewPosition.y += ((1 - cosTheta) * axis.y * axis.z - axis.x * sinTheta) * vPos.z;
 
    vNewPosition.z  = ((1 - cosTheta) * axis.x * axis.z - axis.y * sinTheta) * vPos.x;
    vNewPosition.z += ((1 - cosTheta) * axis.y * axis.z + axis.x * sinTheta) * vPos.y;
    vNewPosition.z += (cosTheta + (1 - cosTheta) * axis.z * axis.z)          * vPos.z;
 
    // ������ ������ �������� ����� ������ � ����� �������
    m_vPosition = vCenter + vNewPosition;
}

void Camera::RotateAroundObject(float angle, CVector3 axis)
{
    CVector3 vNewPosition;
 
    // ������� �����, ������ �������� ����� ��������
	CVector3 vPos = m_vPosition - heroCoords;
 
    // �������� ����� � ������� ����
    float cosTheta = (float)cos(angle);
    float sinTheta = (float)sin(angle);
 
    // ������ �������� ����� ��������
    vNewPosition.x  = (cosTheta + (1 - cosTheta) * axis.x * axis.x)          * vPos.x;
    vNewPosition.x += ((1 - cosTheta) * axis.x * axis.y - axis.z * sinTheta) * vPos.y;
    vNewPosition.x += ((1 - cosTheta) * axis.x * axis.z + axis.y * sinTheta) * vPos.z;
 
    vNewPosition.y  = ((1 - cosTheta) * axis.x * axis.y + axis.z * sinTheta) * vPos.x;
    vNewPosition.y += (cosTheta + (1 - cosTheta) * axis.y * axis.y)          * vPos.y;
    vNewPosition.y += ((1 - cosTheta) * axis.y * axis.z - axis.x * sinTheta) * vPos.z;
 
    vNewPosition.z  = ((1 - cosTheta) * axis.x * axis.z - axis.y * sinTheta) * vPos.x;
    vNewPosition.z += ((1 - cosTheta) * axis.y * axis.z + axis.x * sinTheta) * vPos.y;
    vNewPosition.z += (cosTheta + (1 - cosTheta) * axis.z * axis.z)          * vPos.z;
 
    // ������ ������ �������� ����� ������ � ����� �������
	double top = sqrt((vNewPosition.x*vNewPosition.x + vNewPosition.z*vNewPosition.z)/
		(vNewPosition.x*vNewPosition.x + vNewPosition.y*vNewPosition.y + vNewPosition.z*vNewPosition.z));
	if(top<0.1)
		return;
	m_vPosition = heroCoords + vNewPosition;
	m_vPosition.y = max(m_vPosition.y,1.5);
	m_vView = heroCoords;

	GLfloat l = sqrt(m_vPosition.x*m_vPosition.x + m_vPosition.y*m_vPosition.y + m_vPosition.z*m_vPosition.z);
	if(l>45)
	{
		m_vPosition.x = m_vPosition.x*45.0/l;
		m_vPosition.y = m_vPosition.y*45.0/l;
		m_vPosition.z = m_vPosition.z*45.0/l;
	}
}

void Camera::RotateView(float angle, CVector3 axis)
{
	CVector3 vNewView;
	CVector3 vView;
 
	// ������� ��� ������ ������� (�����������, ���� �� �������)
	vView = m_vView - m_vPosition;
 
	// ������, ���� ������ �������, ���������� � CVector3 "vView", �� ����� ������� ���.
	// ��� �-� ����� ���������, ����� ����� ���������� ������-�������.
	// ����, ��� ����� ��������� ������ ����� �������. ����������� ��� �������� ���:
	// ������, �� ����� �� �����, ������ ������. �� ������� � �����-�� �����, �����?
	// ������, ���� �� �������� ������ ������ ��� �������, ����������� ������� ���������.
	// �������������� ������������ �����, �� ������� �� ������� (������ �������).
	// ��� ������ �� �������� m_vView - ������ ��� ��� � ���� ������ 
	// �������. �� ����� ������� m_vView ������ m_vPosition
	// �� �����, ����� ����������� �� ���.
	//
	// ����� ����������� �������� ������, �� ����� ������������ axis-angle ��������.
	// � ������ ������������, ��� ������� ��� �������� �������� �� ����� ������, ��
	// �������� �� ����� ����. Axis-angle �������� ��������� ��� ������� ����� � ������������
	// ������ ������ ���. ��� ������, ��� �� ����� ����� ���� ����� ������� (m_vView) �
	// ������� ������ ����� �������.
 
	// ���������� 1 ��� ����� � ������� ����������� ����
	float cosTheta = (float)cos(angle);
	float sinTheta = (float)sin(angle);
 
	// ������ ����� ������� ��� ��������� �����
	vNewView.x  = (cosTheta + (1 - cosTheta) * axis.x * axis.x)    	     * vView.x;
	vNewView.x += ((1 - cosTheta) * axis.x * axis.y - axis.z * sinTheta)	 * vView.y;
	vNewView.x += ((1 - cosTheta) * axis.x * axis.z + axis.y * sinTheta)	 * vView.z;
 
	vNewView.y  = ((1 - cosTheta) * axis.x * axis.y + axis.z * sinTheta)	 * vView.x;
	vNewView.y += (cosTheta + (1 - cosTheta) * axis.y * axis.y)    	     * vView.y;
	vNewView.y += ((1 - cosTheta) * axis.y * axis.z - axis.x * sinTheta)	 * vView.z;
 
	vNewView.z  = ((1 - cosTheta) * axis.x * axis.z - axis.y * sinTheta)	 * vView.x;
	vNewView.z += ((1 - cosTheta) * axis.y * axis.z + axis.x * sinTheta) * vView.y;
	vNewView.z += (cosTheta + (1 - cosTheta) * axis.z * axis.z)      	 * vView.z;
 
	// ������ ������ ������� ����� ������ �������� � ����� �������
	m_vView = m_vPosition + vNewView;
}

void Camera::StrafeCamera(float speed)
{ 
	// ������� ������ ������� � �������
	m_vPosition.x += m_vStrafe.x * speed;
	m_vPosition.z += m_vStrafe.z * speed;
 
	// ������� ������ � �������
	m_vView.x += m_vStrafe.x * speed;
	m_vView.z += m_vStrafe.z * speed;

	GLfloat l = sqrt(m_vPosition.x*m_vPosition.x + m_vPosition.y*m_vPosition.y + m_vPosition.z*m_vPosition.z);
	if(l>45)
	{
		m_vPosition.x = m_vPosition.x*45.0/l;
		m_vPosition.z = m_vPosition.z*45.0/l;
	}
}
 
void Camera::MoveCamera(float speed)
{
	CVector3 vVector = Normalize(m_vView - m_vPosition);

	m_vPosition.x += vVector.x * speed;
	m_vPosition.z += vVector.z * speed;
	m_vView.x += vVector.x * speed;
	m_vView.z += vVector.z * speed;
	
	GLfloat l = sqrt(m_vPosition.x*m_vPosition.x + m_vPosition.y*m_vPosition.y + m_vPosition.z*m_vPosition.z);
	if(l>45)
	{
		m_vPosition.x = m_vPosition.x*45.0/l;
		m_vPosition.z = m_vPosition.z*45.0/l;
	}
}

void Camera::MoveUp(float speed)
{
	m_vPosition.y += speed; 
	m_vPosition.y = max(m_vPosition.y,1.5);
	m_vView = heroCoords;

	GLfloat l = sqrt(m_vPosition.x*m_vPosition.x + m_vPosition.y*m_vPosition.y + m_vPosition.z*m_vPosition.z);
	if(l>45)
	{
		m_vPosition.x = m_vPosition.x*45.0/l;
		m_vPosition.y = m_vPosition.y*45.0/l;
		m_vPosition.z = m_vPosition.z*45.0/l;
	}
}

void Camera::ZoomCamera (float speed) {

}

void Camera::Update()
{
	// ���� �� ������������ ������ ������� ������ ���, ����� �� ��������
	// ������. ��� ����������, �.�. �������� ������������ �� ������ ������.
 
	// ����. ���������� ��� ���������� cross 
	CVector3 vCross = Cross(m_vView - m_vPosition, m_vUpVector);
 
	//����������� ������ �������
	m_vStrafe = Normalize(vCross);
 
}

void Camera::Look()
{
	// ����� OpenGL �������, ������ � ����. ������ ������
	glLoadIdentity();
	gluLookAt(m_vPosition.x, m_vPosition.y, m_vPosition.z,
			  m_vView.x,	m_vView.y,     m_vView.z,
			  m_vUpVector.x, m_vUpVector.y, m_vUpVector.z);
}

void Camera::SetHeroCoords(CVector3 _coords) {
	heroCoords = _coords;
}