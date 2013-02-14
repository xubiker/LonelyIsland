#include "Hero.h"

Hero::Hero ()
{
	cam = NULL;
	coords = CVector3(0, 1, 0);
}

void Hero::AttachCamera (Camera* _cam)
{
	cam = _cam;
	cam->SetHeroCoords(coords);
	std::cout << "camera attached" << std::endl;
}

void Hero::Move (float speed)
{
	if (cam != NULL) {
		CVector3 vVector = CVector3::Normalize(cam->m_vView - cam->m_vPosition);
		coords.x += vVector.x * speed;
		coords.z += vVector.z * speed;
		cam->SetHeroCoords(coords);
		cam->MoveCamera(speed);
		coords = cam->m_vView;
	}
}

void Hero::MoveUp (float speed)
{
	if (cam != NULL) {
		CVector3 vVector = CVector3::Normalize(cam->m_vView - cam->m_vPosition);
		coords.y += speed;
		coords.y = max(coords.y, 1.0);
		cam->SetHeroCoords(coords);
		cam->MoveUp(speed);
	}
}

void Hero::Strafe (float speed)
{
	if (cam != NULL) {
		CVector3 vVector = CVector3::Normalize(cam->m_vView - cam->m_vPosition);
		coords.x += cam->m_vStrafe.x * speed;
		coords.z += cam->m_vStrafe.z * speed;
		cam->SetHeroCoords(coords);
		cam->StrafeCamera(speed);
		coords = cam->m_vView;
	}
}

void Hero::Draw (double curTime)
{
    glPushMatrix();
    glTranslatef(coords.x, coords.y, coords.z);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(1.0, 0.0, 0.0);
	
	glEnable(GL_AUTO_NORMAL);
	glDisable(GL_LIGHT0);
	glMaterialf(GL_FRONT, GL_SHININESS, 0);
	glTranslatef(0.0, 0.5 * sin(2 * curTime), 0.0);
	glScalef(1.0, 1.0 + 0.1 * sin(2 * curTime), 1.0);
	glutSolidSphere(0.6, 20, 20);
    
    glPopMatrix();
}