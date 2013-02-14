// Включаем наш хидер
#include "CVector3.h"
#include "Camera.h"

#include <iostream>

#define kSpeed 0.1f

 
inline CVector3 Cross(CVector3 vVector1, CVector3 vVector2)
{
	/////
	/////	Возвращает перпендикулярный вектор от 2х переданных векторов.
	/////   2 любых пересекающихся вектора образуют плоскость, от котороый мы
	/////   и ищем перпендикуляр.
	/////
	CVector3 vNormal;
 
	// Если у нас есть 2 вектора (вектор взгляда и вертикальный вектор), 
	// у нас есть плоскость, от которой мы можем вычислить угол в 90 градусов.
	// Рассчет cross'a прост, но его сложно запомнить с первого раза. 
	vNormal.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));
 
	vNormal.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));
 
	vNormal.z  = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));
 
	// *ВАЖНО* Вы не можете менять этот порядок, иначе ничего не будет работать.
	// Должно быть именно так, как здесь. Просто запомните, если вы ищите Х, вы не
	// используете значение X двух векторов, и то же самое для Y и Z. Заметьте,
	// вы рассчитываете значение из двух других осей, и никогда из той же самой.
 
	// Итак, зачем всё это? Нам нужно найти ось, вокруг которой вращаться. Вращение камеры
	// влево и вправо простое - вертикальная ось всегда (0,1,0). 
	// Вращение камеры вверх и вниз отличается, так как оно происходит вне 
	// глобальных осей.
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
	m_vPosition = CVector3(0.0, 10.0, 0.0);		//Инициализируем вектор нашей позиции в нулевые координаты
	m_vView = CVector3(0.0, 11.0, 0.5);  		//Иниц. вектор взгляда
	m_vUpVector = CVector3(0.0, 0.0, 1.0);		//Вектор верт.

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

	// Получим центр, вокруг которого нужно вращатся
    CVector3 vPos = m_vPosition - vCenter;
 
    // Вычислим синус и косинус угла
    float cosTheta = (float)cos(angle);
    float sinTheta = (float)sin(angle);
 
    // Найдем значение точки вращения
    vNewPosition.x  = (cosTheta + (1 - cosTheta) * axis.x * axis.x)          * vPos.x;
    vNewPosition.x += ((1 - cosTheta) * axis.x * axis.y - axis.z * sinTheta) * vPos.y;
    vNewPosition.x += ((1 - cosTheta) * axis.x * axis.z + axis.y * sinTheta) * vPos.z;
 
    vNewPosition.y  = ((1 - cosTheta) * axis.x * axis.y + axis.z * sinTheta) * vPos.x;
    vNewPosition.y += (cosTheta + (1 - cosTheta) * axis.y * axis.y)          * vPos.y;
    vNewPosition.y += ((1 - cosTheta) * axis.y * axis.z - axis.x * sinTheta) * vPos.z;
 
    vNewPosition.z  = ((1 - cosTheta) * axis.x * axis.z - axis.y * sinTheta) * vPos.x;
    vNewPosition.z += ((1 - cosTheta) * axis.y * axis.z + axis.x * sinTheta) * vPos.y;
    vNewPosition.z += (cosTheta + (1 - cosTheta) * axis.z * axis.z)          * vPos.z;
 
    // Теперь просто прибавим новый вектор к нашей позиции
    m_vPosition = vCenter + vNewPosition;
}

void Camera::RotateAroundObject(float angle, CVector3 axis)
{
    CVector3 vNewPosition;
 
    // Получим центр, вокруг которого нужно вращатся
	CVector3 vPos = m_vPosition - heroCoords;
 
    // Вычислим синус и косинус угла
    float cosTheta = (float)cos(angle);
    float sinTheta = (float)sin(angle);
 
    // Найдем значение точки вращения
    vNewPosition.x  = (cosTheta + (1 - cosTheta) * axis.x * axis.x)          * vPos.x;
    vNewPosition.x += ((1 - cosTheta) * axis.x * axis.y - axis.z * sinTheta) * vPos.y;
    vNewPosition.x += ((1 - cosTheta) * axis.x * axis.z + axis.y * sinTheta) * vPos.z;
 
    vNewPosition.y  = ((1 - cosTheta) * axis.x * axis.y + axis.z * sinTheta) * vPos.x;
    vNewPosition.y += (cosTheta + (1 - cosTheta) * axis.y * axis.y)          * vPos.y;
    vNewPosition.y += ((1 - cosTheta) * axis.y * axis.z - axis.x * sinTheta) * vPos.z;
 
    vNewPosition.z  = ((1 - cosTheta) * axis.x * axis.z - axis.y * sinTheta) * vPos.x;
    vNewPosition.z += ((1 - cosTheta) * axis.y * axis.z + axis.x * sinTheta) * vPos.y;
    vNewPosition.z += (cosTheta + (1 - cosTheta) * axis.z * axis.z)          * vPos.z;
 
    // Теперь просто прибавим новый вектор к нашей позиции
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
 
	// Получим наш вектор взгляда (направление, куда мы смотрим)
	vView = m_vView - m_vPosition;
 
	// Теперь, имея вектор взгляда, хранящийся в CVector3 "vView", мы можем вращать его.
	// Эта ф-я будет вызыватся, когда нужно повернутся налево-направо.
	// Итак, нам нужно вращаться вокруг нашей позиции. Представьте это примерно так:
	// скажем, мы стоим на месте, смотря вперед. Мы смотрим в какую-то точку, верно?
	// Теперь, если мы повернем голову налево или направо, направление взгляда изменится.
	// Соответственно переместится точка, на которую мы смотрим (вектор взгляда).
	// Вот почему мы изменяем m_vView - потому что это и есть вектор 
	// взгляда. Мы будем вращать m_vView вокруг m_vPosition
	// по кругу, чтобы реализовать всё это.
	//
	// Чтобы реализовать вращение камеры, мы будем использовать axis-angle вращение.
	// Я должен предупредить, что формулы для рассчета вращения не очень просты, но
	// занимают не много кода. Axis-angle вращение позволяет нам вращать точку в пространстве
	// вокруг нужной оси. Это значит, что мы можем взять нашу точку взгляда (m_vView) и
	// вращать вокруг нашей позиции.
 
	// Рассчитаем 1 раз синус и косинус переданного угла
	float cosTheta = (float)cos(angle);
	float sinTheta = (float)sin(angle);
 
	// Найдем новую позицию для вращаемой точки
	vNewView.x  = (cosTheta + (1 - cosTheta) * axis.x * axis.x)    	     * vView.x;
	vNewView.x += ((1 - cosTheta) * axis.x * axis.y - axis.z * sinTheta)	 * vView.y;
	vNewView.x += ((1 - cosTheta) * axis.x * axis.z + axis.y * sinTheta)	 * vView.z;
 
	vNewView.y  = ((1 - cosTheta) * axis.x * axis.y + axis.z * sinTheta)	 * vView.x;
	vNewView.y += (cosTheta + (1 - cosTheta) * axis.y * axis.y)    	     * vView.y;
	vNewView.y += ((1 - cosTheta) * axis.y * axis.z - axis.x * sinTheta)	 * vView.z;
 
	vNewView.z  = ((1 - cosTheta) * axis.x * axis.z - axis.y * sinTheta)	 * vView.x;
	vNewView.z += ((1 - cosTheta) * axis.y * axis.z + axis.x * sinTheta) * vView.y;
	vNewView.z += (cosTheta + (1 - cosTheta) * axis.z * axis.z)      	 * vView.z;
 
	// Теперь просто добавим новый вектор вращения к нашей позиции
	m_vView = m_vPosition + vNewView;
}

void Camera::StrafeCamera(float speed)
{ 
	// добавим вектор стрейфа к позиции
	m_vPosition.x += m_vStrafe.x * speed;
	m_vPosition.z += m_vStrafe.z * speed;
 
	// Добавим теперь к взгляду
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
	// Ниже мы рассчитываем вектор стрейфа каждый раз, когда мы апдейтим
	// камеру. Это необходимо, т.к. значения используются во многих местах.
 
	// Иниц. переменную для результата cross 
	CVector3 vCross = Cross(m_vView - m_vPosition, m_vUpVector);
 
	//Нормализуем вектор стрейфа
	m_vStrafe = Normalize(vCross);
 
}

void Camera::Look()
{
	// Дадим OpenGL позицию, взгляд и верт. вектор камеры
	glLoadIdentity();
	gluLookAt(m_vPosition.x, m_vPosition.y, m_vPosition.z,
			  m_vView.x,	m_vView.y,     m_vView.z,
			  m_vUpVector.x, m_vUpVector.y, m_vUpVector.z);
}

void Camera::SetHeroCoords(CVector3 _coords) {
	heroCoords = _coords;
}