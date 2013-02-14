#include <gl/glew.h>
#include <gl/glut.h>
#include <gl/freeglut.h>

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>

#include "BmpLoad.h"
#include "CVector3.h"
#include "camera.h"
#include "Hero.h"
#include "Platforms.h"
#include "wchar.h"
#include "Shader.h"
#include "Terrain.h"
#include "Snow.h"
#include "Cloud.h"
#include "Sprite.h"
#include "Bird.h"

//#include "include/math/math3d.h"
//#include "include/math/mathgl.h"

using namespace std;

#define kspeed 0.5f

Camera* Camera3p;
Hero* mHero;
vector<Platform*> plats;
Shader* shdr;
Shader* postShdr;
Shader* blurShdr;
Terrain* terrain;
vector<Sprite*> sprites;
int winWidth = 1200;
int winHeight = 800;
float focus = 0.8;
bool dragging = false;

void reshape (int w, int h)
{
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, (GLsizei)winWidth, (GLsizei)winHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0, (GLfloat)winWidth / winHeight, 1, 200.0);
	glMatrixMode(GL_MODELVIEW);
}

bool mouse_move = true;
bool smoothStrafeLeft = false;

int smoothStrafe = 10, smoothCoeff = 1;

GLuint depthTexture[4];
GLuint postTexture, postDepthTexture, postDepthTexture1, glassTexture, blurTexture;
GLfloat *temp;
GLfloat *lightModelview[4], *lightProj[4], *modelMatrix;
clock_t time0;
GLfloat curTime;

void renderSprites()
{
	vector<pair<double,int>> forSort;
	for (int i = 0; i < sprites.size(); ++i)
		forSort.push_back(make_pair(sprites[i]->getDistance(*Camera3p),i));
	std::sort(forSort.begin(), forSort.end());
	for(int i = forSort.size() - 1; i >= 0; --i)
		sprites[forSort[i].second]->render(*Camera3p);
}

GLuint skyBack,skyFront, skyLeft,skyRight,skyTop,skyBottom;

void LoadTex(GLuint* tex, const char* path)
{
	int tex_width, tex_height;
	glGenTextures(1, tex);
	unsigned char *tex_bits;	
	
	if((tex_bits = LoadTrueColorBMPFile(path, &tex_width, &tex_height)) == NULL)
		throw "Cannot open texture file";
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, *tex);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_bits);
	// Устанавливаем параметры текстуры
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	delete[] tex_bits;
}

void LoadSkyBox()
{
	LoadTex(&skyBack,   "skybox/back.bmp");
	LoadTex(&skyFront,  "skybox/front.bmp");
	LoadTex(&skyLeft,   "skybox/left.bmp");
	LoadTex(&skyRight,  "skybox/right.bmp");
	LoadTex(&skyTop,    "skybox/top.bmp");
	LoadTex(&skyBottom, "skybox/bottom.bmp");
}

void sky()
{
	glMatrixMode(GL_PROJECTION_MATRIX);
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW_MATRIX);
	glPushMatrix();
	glTranslatef(Camera3p->Position().x, Camera3p->Position().y, Camera3p->Position().z);
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	float size = 100.0;
	glBindTexture(GL_TEXTURE_2D, skyBack);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3d (-size, -size, size - 0.1);
		glTexCoord2f(0.0f, 1.0f); glVertex3d (-size,  size, size - 0.1);
		glTexCoord2f(1.0f, 1.0f); glVertex3d ( size,  size, size - 0.1);
		glTexCoord2f(1.0f, 0.0f); glVertex3d ( size, -size, size - 0.1);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, skyFront);
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 0.0f); glVertex3d (-size, -size, -size + 0.1);
		glTexCoord2f(1.0f, 1.0f); glVertex3d (-size,  size, -size + 0.1);
		glTexCoord2f(0.0f, 1.0f); glVertex3d ( size,  size, -size + 0.1);
		glTexCoord2f(0.0f, 0.0f); glVertex3d ( size, -size, -size + 0.1);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, skyRight);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3d (-size + 0.1, -size, -size);
		glTexCoord2f(0.0f, 1.0f); glVertex3d (-size + 0.1,  size, -size);
		glTexCoord2f(1.0f, 1.0f); glVertex3d (-size + 0.1,  size,  size);
		glTexCoord2f(1.0f, 0.0f); glVertex3d (-size + 0.1, -size,  size);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, skyLeft);
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 0.0f); glVertex3d (size - 0.1, -size, -size);
		glTexCoord2f(1.0f, 1.0f); glVertex3d (size - 0.1,  size, -size);
		glTexCoord2f(0.0f, 1.0f); glVertex3d (size - 0.1,  size,  size);
		glTexCoord2f(0.0f, 0.0f); glVertex3d (size - 0.1, -size,  size);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, skyTop);
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 0.0f); glVertex3d (-size, size - 0.1, -size);
		glTexCoord2f(0.0f, 0.0f); glVertex3d ( size, size - 0.1, -size);
		glTexCoord2f(0.0f, 1.0f); glVertex3d ( size, size - 0.1,  size);
		glTexCoord2f(1.0f, 1.0f); glVertex3d (-size, size - 0.1,  size);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, skyBottom);
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 0.0f); glVertex3d (-size, -size + 0.1, -size);
		glTexCoord2f(0.0f, 0.0f); glVertex3d ( size, -size + 0.1, -size);
		glTexCoord2f(0.0f, 1.0f); glVertex3d ( size, -size + 0.1,  size);
		glTexCoord2f(1.0f, 1.0f); glVertex3d (-size, -size + 0.1,  size);
	glEnd();
	glPopMatrix();
	glPopMatrix();
}

void RenderScene()
{
	terrain->draw();
	
	shdr->setupTexture(0, "diffuseMap",shdr->texID[1]);
	mHero->Draw(curTime);
	shdr->setupTexture(0, "diffuseMap",shdr->texID[2]);
	for(int i = 0; i < (int)plats.size(); i++)
		plats[i]->Draw();
	glUseProgram(0);
	sky();
	renderSprites();
}

clock_t start;
bool doBlur = false;

void ColorBlur()
{
	clock_t cur = clock();
	if ((cur - start) / CLOCKS_PER_SEC > 2)
		doBlur = false;
	glUniform1f(glGetUniformLocation(blurShdr->program, "time"), ((GLfloat)cur - start) / CLOCKS_PER_SEC);
	glUniform1i(glGetUniformLocation(blurShdr->program, "doBlur"), (GLint)doBlur);
}

//Time-dependent snow initialization
int nSnow;

void initSnow()
{
	if(curTime > 50)
		return;
	int t = curTime * 5;
	for (int i = 0; i < t - nSnow; ++i)
		sprites.push_back(new Snowflake(curTime));
	nSnow = t;
}

void display()
{
	curTime = ((GLfloat)clock() - time0) / CLOCKS_PER_SEC;
	initSnow();

	glUseProgram(shdr->program);
	glViewport(0, 0, 1500, 1500);
	glEnable(GL_DEPTH_BUFFER_BIT);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_BACK);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, 1.0, 1.0, 200.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(50.0, 1.0, 0.0, 0.0);
	glRotatef(10.0, 0.0, 1.0, 0.0);
	glTranslatef(-15.0, -80.0, -28.0);
	glViewport(0, 0, 1500, 1500);
	//glGetFloatv(GL_PROJECTION_MATRIX,lightProj[0]);
	//glGetFloatv(GL_MODELVIEW_MATRIX,lightModelview[0]);
	
	//Render for shadows
	RenderScene();
	
	//Get depth map
	glBindTexture(GL_TEXTURE_2D, depthTexture[0]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 0, 0, 1500, 1500, 0);
	
	glViewport(0, 0, winWidth, winHeight);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_BACK);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (GLfloat)winWidth / winHeight, 1.0, 200.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	Camera3p->Look();
	
	glUseProgram(shdr->program);
	glGetFloatv(GL_MODELVIEW_MATRIX, modelMatrix);
	glUniformMatrix4fv(glGetUniformLocation(shdr->program, "modelMatrix"), 1, GL_TRUE, modelMatrix);
	glUniformMatrix4fv(glGetUniformLocation(shdr->program, "lightModelview1"), 1, GL_TRUE, lightModelview[0]);
	glUniformMatrix4fv(glGetUniformLocation(shdr->program, "lightProj1"), 1, GL_TRUE, lightProj[0]);
	glUniformMatrix4fv(glGetUniformLocation(shdr->program, "modelMatrix"), 1, GL_TRUE, modelMatrix);
	float v1[] = { 1, 0, 0, 0,
				   0, 1, 0, 0,
				   0, 0, 1, 0,
				   0, 0, 0, 1 };
	glUniformMatrix4fv(glGetUniformLocation(shdr->program, "mirror"), 1, GL_TRUE, v1);
	shdr->setupTexture(0, "diffuseMap", shdr->texID[0]);
	shdr->setupTexture(2, "shadowTexture1", depthTexture[0]);
	
	// Render scene
	RenderScene();
	
	// Render mirror
	glUseProgram(0);
	glClear(GL_STENCIL_BUFFER_BIT);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, 0);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glBegin(GL_QUADS);
		glColor4f(0.0, 0.0, 0.0, 1.0);
		glVertex3f(-50.0, -0.01, -50.0);
		glVertex3f(-50.0, -0.01,  50.0);
		glVertex3f( 50.0, -0.01,  50.0);
		glVertex3f( 50.0, -0.01, -50.0);
    glEnd();
	
	// Get mirror depth texture
	glBindTexture(GL_TEXTURE_2D, postDepthTexture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 0, 0, winWidth, winHeight, 0);
	
	glStencilFunc(GL_EQUAL, 0x1, 0xffffffff);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glEnable(GL_TEXTURE_2D);
	glClear(GL_DEPTH_BUFFER_BIT);
	
	float v2[] = { 1,  0,  0,  0,
				   0, -1,  0,  0,
				   0,  0,  1,  0,
				   0,  0,  0,  1 };
	glUseProgram(shdr->program);
	glUniformMatrix4fv(glGetUniformLocation(shdr->program, "mirror"), 1, GL_TRUE, v2);
	shdr->setupTexture(2, "shadowTexture1",depthTexture[0]);
	shdr->setupTexture(0, "diffuseMap",shdr->texID[0]);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (GLfloat)winWidth / winHeight, 1.0, 200.0);	
	glMatrixMode(GL_MODELVIEW);
	Camera3p->Look();

	// Render scene in mirror
	glMultMatrixf(v2);
	Camera3p->mirror = -1;
	RenderScene();
	Camera3p->mirror = 1;
	
	// Get textures of rendered scene
	glBindTexture(GL_TEXTURE_2D, postTexture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16, 0, 0, winWidth, winHeight, 0);

	glBindTexture(GL_TEXTURE_2D, postDepthTexture1);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 0, 0, winWidth, winHeight, 0);

	glUseProgram(postShdr->program);
	postShdr->setupTexture(1, "texture",postTexture);
	postShdr->setupTexture(0, "depth",postDepthTexture1);
	glUniform1f(glGetUniformLocation(postShdr->program, "time"), ((GLfloat)clock() - start) / CLOCKS_PER_SEC);
	glUniform1f(glGetUniformLocation(postShdr->program, "focus"), focus);
	glUniform1f(glGetUniformLocation(postShdr->program, "glass"), 0);
	glUniform1f(glGetUniformLocation(postShdr->program, "winWidth"), winWidth);
	glUniform1f(glGetUniformLocation(postShdr->program, "winHeight"), winHeight);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, 1, 0, 1, 0, 1);
	glDisable(GL_DEPTH_TEST);

	// Render for blur
	//ColorBlur();
	glBegin(GL_QUADS);
		glVertex3d (-1, -1, 0.1);
		glVertex3d (-1,  2, 0.1);
		glVertex3d ( 2,  2, 0.1);
		glVertex3d ( 2, -1, 0.1);
	glEnd();

	// Render for blur in mirror
	glStencilFunc(GL_NOTEQUAL, 0x1, 0xffffffff);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	postShdr->setupTexture(0, "depth", postDepthTexture);
	glBegin(GL_QUADS);
		glVertex3d (-1, -1, 0.1);
		glVertex3d (-1,  2, 0.1);
		glVertex3d ( 2,  2, 0.1);
		glVertex3d ( 2, -1, 0.1);
	glEnd();

	// Get texture for water
	glBindTexture(GL_TEXTURE_2D, glassTexture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16, 0, 0, winWidth, winHeight, 0);

	// Render water
	glUseProgram(postShdr->program);
	postShdr->setupTexture(1, "texture", glassTexture);
	postShdr->setupTexture(0, "depth", postDepthTexture);
	glUniform1f(glGetUniformLocation(postShdr->program, "glass"), 1);
	glUniformMatrix4fv(glGetUniformLocation(postShdr->program, "modelMatrix"), 1, GL_TRUE, modelMatrix);
	glStencilFunc(GL_EQUAL, 0x1, 0xffffffff);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glBegin(GL_QUADS);
		glVertex3d (-1, -1, 0.1);
		glVertex3d (-1,  2, 0.1);
		glVertex3d ( 2,  2, 0.1);
		glVertex3d ( 2, -1, 0.1);
	glEnd();

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);

	glBindTexture(GL_TEXTURE_2D, blurTexture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16, 0, 0, winWidth, winHeight, 0);
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(blurShdr->program);
	glUniform1f(glGetUniformLocation(blurShdr->program, "winWidth"), winWidth);
	glUniform1f(glGetUniformLocation(blurShdr->program, "winHeight"), winHeight);
	blurShdr->setupTexture(0, "texture", blurTexture);
	ColorBlur();
	glBegin(GL_QUADS);
		glVertex3d (-1, -1, 0.1);
		glVertex3d (-1,  2, 0.1);
		glVertex3d ( 2,  2, 0.1);
		glVertex3d ( 2, -1, 0.1);
	glEnd();
	
	glPopMatrix();

	if (mouse_move) {
		Camera3p->SetViewByMouse();
		Camera3p->Update();
	}
	if (smoothStrafe > 0) {
		Camera3p->StrafeCamera(-kspeed / smoothCoeff);
		smoothStrafe--;
	}

	for (size_t i = 0; i < sprites.size(); ++i)
		sprites[i]->move(curTime);

	glutSwapBuffers();
}
 
void init(void)
{
	time0 = clock();
	LoadSkyBox();
	Cloud::init();
	Snowflake::init();
	Bird::init();

	for (int i = 0; i < 500; ++i)
		sprites.push_back(new Cloud());
	sprites.push_back(new Bird());

	for (int i = 0; i < 4; ++i)
	{
		lightModelview[i] = new GLfloat[16];
		lightProj[i] = new GLfloat[16];
		glGenTextures(1, depthTexture + i);
	}
	glGenTextures(1, &postTexture);
	glGenTextures(1, &postDepthTexture);
	glGenTextures(1, &postDepthTexture1);
	glGenTextures(1, &glassTexture);
	glGenTextures(1, &blurTexture);
	modelMatrix = new GLfloat[16];
	terrain = new Terrain(257);
	try {
		terrain->BuildFractalMap();
	} catch (string x) {

	}
	shdr = new Shader();
	shdr->initShader("TextureVertex.glsl", "TextureFragment.glsl");
	postShdr = new Shader();
	postShdr->initShader("PosteffectV.glsl", "PosteffectF.glsl");
	blurShdr = new Shader();
	blurShdr->initShader("BlurV.glsl", "BlurF.glsl");

	for (int i = -2; i < 3; i++) {
		for (int j = -2; j < 3; j++) {
			Platform* p = new Platform(CVector3(3 * i, (i + j) % 3, 3 * j));
 			plats.push_back(p);
		}
	}

	Camera3p = new Camera();
	//	Position, Direction, Vertical
	Camera3p->PositionCamera(CVector3(0, 15, 10),   CVector3(0, 5, -10), CVector3(0, 1, 0));
	Camera3p->UpdateCursorPos();
	mHero = new Hero();
	mHero->AttachCamera(Camera3p);

	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glViewport(0, 0, 1500, 1500);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, 1.0, 1.0, 200.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(50.0, 1.0, 0.0, 0.0);
	glRotatef(10.0, 0.0, 1.0, 0.0);
	glTranslatef(-15.0, -80.0, -28.0);
	glGetFloatv(GL_PROJECTION_MATRIX, lightProj[0]);
	glGetFloatv(GL_MODELVIEW_MATRIX, lightModelview[0]);
}
 
void idle()
{
	static int t = 0;
	if (glutGet(GLUT_ELAPSED_TIME) - t > 1) {
		t = glutGet(GLUT_ELAPSED_TIME);
		for (int i = 0; i < (int)plats.size(); i++) {
			plats[i]->nextPos(curTime);
		}
		glutPostRedisplay();
	}
}

void mouse_wheel (int button, int dir, int x, int y)
{
	if (dir > 0)
		Camera3p->MoveCamera(kspeed); // Zoom in
	else
		Camera3p->MoveCamera(-kspeed); // Zoom out
}

void keyboard (unsigned char key, int x, int y)
{
	switch(key) {
		case 27:
			exit(0);
			break;
		case 'a':
//			smoothStrafe = 20;
//			smoothCoeff = 20;
			mHero->Strafe(-kspeed);
			break;
		case 'd':
			mHero->Strafe(kspeed);
			break;
		case 'w':
			mHero->Move(kspeed);
			break;
		case 's':
			mHero->Move(-kspeed);
			break;
		case 'r':
			mHero->MoveUp(kspeed);
			break;
		case 'f':
			mHero->MoveUp(-kspeed);
			break;
		case 'm':
			mouse_move = !mouse_move;
			break;
		case 'j':
			Camera3p->MoveCamera(-kspeed);
			break;
		case 'g':
			Camera3p->RotateAroundObject(kspeed / 10, CVector3(0, 1, 0));
			break;
		case 'o':
			focus += (1.0 - focus) / 3;
			break;
		case 'l':
			focus -= (1.0 - focus) / 3;
			break;
		case 'b':
			doBlur = true;
			start = clock();
			break;
	}
}

int main (int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);

	glutInitWindowPosition(0, 0);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow(argv[0]);
	glutSetCursor(GLUT_CURSOR_NONE);

	glewInit();
	init();
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseWheelFunc(mouse_wheel);
	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
} 
