#include <GL/glut.h>
#include <GL/freeglut.h>

#include <iostream>
#include <math.h>
#include <time.h>

#include "BmpLoad.h"

class Terrain {
	private:
		int mapSize;
		int** heightMap;
		float* elements;
		GLuint* indices;
		float PerlinNoise_2D (float x, float y);
		float Cubic_Interpolate(float v0, float v1, float v2, float v3, int x);
		float Cosine_Interpolate (float a, float b, int x);
		float interpolatedSmoothNoise(float x, float y);
		float smoothNoise(int x, int y);
		inline float noise(int x, int y);
		void BuildFractalMap_ (int x1, int y1, int x2, int y2);

	public:
		Terrain (int size);
		void NormalizeMap ();
		void SharpenMap ();
		void SmoothMap ();
		void BuildFractalMap ();
		void generate ();
		void generate2 ();
		void draw ();
		void drawInit ();
		~Terrain ();
};

void RenderLandscape();