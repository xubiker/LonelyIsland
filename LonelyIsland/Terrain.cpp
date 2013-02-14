#include "Terrain.h"
#include <algorithm>

const double M_PI = 3.14159265358979323846;

using namespace std;

Terrain::Terrain (int size)
{
	mapSize = size;
	heightMap = new int*[mapSize];
	for (int i = 0; i < mapSize; i++) {
		heightMap[i] = new int[mapSize];
	}
	srand ( time(NULL) );
}

Terrain::~Terrain ()
{
	for (int i = 0; i < mapSize; i++)
		delete heightMap[i];
	delete heightMap;
}


void Terrain::NormalizeMap()
{
	int min = MAXINT, max = MININT;
	for (int i = 0; i < mapSize; i++) {
		for (int j = 0; j < mapSize; j++) {
			if (heightMap[i][j] > max)
				max = heightMap[i][j];
			else if (heightMap[i][j] < min)
				min = heightMap[i][j];
		}
	}
	double parabol = sqrt((mapSize / 4) * (mapSize / 4) + (mapSize / 4) * (mapSize / 4));
	for (int i = 0; i < mapSize; i++) {
		for (int j = 0; j < mapSize; j++) {
			heightMap[i][j] = (heightMap[i][j] - min) * 255 / (max - min);
			if(sqrt((i - mapSize / 2) * (i - mapSize / 2) + (j - mapSize / 2) * (j - mapSize / 2)) < mapSize / 3)
				heightMap[i][j] = heightMap[i][j] * sqrt((i - mapSize / 2) * (i - mapSize / 2) + 
								  (j - mapSize / 2) * (j - mapSize / 2)) * 3 / mapSize + 1;
			heightMap[i][j] *= sqrt(sqrt(std::min(std::min(i, mapSize - 1 - i), std::min(j, mapSize - 1 - j)) * 2.0 / mapSize));
		}
	}
}

void Terrain::SharpenMap ()
{
	for (int i = 0; i < mapSize; i++){
		for (int j = 0; j < mapSize; j++) {
			heightMap[i][j] = heightMap[i][j] * heightMap[i][j];
		}
	}
}

void Terrain::SmoothMap()
{
	int** heightMap_;
	heightMap_ = new int*[mapSize];
	for (int i = 0; i < mapSize; i++)
		heightMap_[i] = new int[mapSize];

	for (int i = 1; i < mapSize - 1; i++){
		for (int j = 1; j < mapSize - 1; j++) {
			heightMap_[i][j] = (heightMap[i-1][j] + heightMap[i+1][j] + heightMap[i][j-1] + heightMap[i][j+1]) / 4;
		}
	}

	for (int i = 1; i < mapSize - 1; i++)
		for (int j = 1; j < mapSize - 1; j++)
			heightMap[i][j] = heightMap_[i][j];

	for (int i = 0; i < mapSize; i++) {
		delete heightMap_[i];
	}
	delete heightMap_;
}

void Terrain::BuildFractalMap ()
{
	BuildFractalMap_(0, 0, mapSize - 1, mapSize - 1);
	NormalizeMap();
	SmoothMap();
	SharpenMap();
	NormalizeMap();
	drawInit();
}

void Terrain::BuildFractalMap_(int x1, int y1, int x2, int y2)
{
	const float NoizeFactor = 0.5;
	int l = x2 - x1;
	int x = (x1 + x2) / 2;
	int y = (y1 + y2) / 2;
	if (l == mapSize - 1) {
		for (int i = 0; i < mapSize; i++){
			for (int j = 0; j < mapSize; j++) {
				heightMap[i][j] = MININT;
			}
		}
		heightMap[x1][y1] = 0;
		heightMap[x2][y1] = 0;
		heightMap[x1][y2] = 0;
		heightMap[x2][y2] = 0;
	}
	if (heightMap[x][y1] == MININT) {
	    heightMap[x][y1] = (heightMap[x1][y1] + heightMap[x2][y1]) / 2
							+ (-((double) rand() / (RAND_MAX)) * 2  + 1) * l * NoizeFactor;
	}
	if (heightMap[x1][y] == MININT) {
	    heightMap[x1][y] = (heightMap[x1][y1] + heightMap[x1][y2]) / 2
							+ (-((double) rand() / (RAND_MAX)) * 2  + 1) * l * NoizeFactor;
	}
	if (heightMap[x2][y] == MININT) {
	    heightMap[x2][y] = (heightMap[x2][y1] + heightMap[x2][y2]) / 2
							+ (-((double) rand() / (RAND_MAX)) * 2  + 1) * l * NoizeFactor;
	}
	if (heightMap[x][y2] == MININT) {
	    heightMap[x][y2] = (heightMap[x1][y2] + heightMap[x2][y2]) / 2
							+ (-((double) rand() / (RAND_MAX)) * 2  + 1) * l * NoizeFactor;
	}
	if (heightMap[x][y] == MININT) {
	    heightMap[x][y] = (heightMap[x][y1] + heightMap[x][y2] + heightMap[x1][y] + heightMap[x2][y]) / 4
							+ (-((double) rand() / (RAND_MAX)) * 2  + 1) * l * NoizeFactor;
	}
	if (l > 1) {
	    BuildFractalMap_ ( x1, y1, x,  y  );
		BuildFractalMap_ ( x,  y1, x2, y  );
	    BuildFractalMap_ ( x1, y,  x,  y2 );
		BuildFractalMap_ ( x,  y,  x2, y2 );
	}
}

void Terrain::generate2 ()
{
	unsigned char* tex_bits;
	int width = 400, height = 400;
	if((tex_bits = LoadTrueColorBMPFile("heightmap.bmp", &width, &height)) == NULL)
		throw std::exception("Cannot open texture file");
	for (int i = 0; i < mapSize; i++)
		for (int j = 0; j < mapSize; j++)
			heightMap[i][j] = (float)tex_bits[(i*width + j)*3 +1] / 4;
	delete[] tex_bits;
}

void Terrain::drawInit ()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	GLfloat* v = new GLfloat[mapSize*mapSize*3];
	GLfloat* t = new GLfloat[mapSize*mapSize*2];
	indices = new GLuint[(mapSize-1)*(mapSize-1)*4];

	const GLfloat TextureBit = 1.0f/(float)mapSize;
	float x, z;
	float cellSize = 0.2;
	float hScale = 0.1;

	for (int i = 0; i < mapSize; i++) {
		for (int j = 0; j < mapSize; j++) { 
			v[i * mapSize * 3 + j * 3]     = cellSize * (i - mapSize / 2);
			v[i * mapSize * 3 + j * 3 + 1] = hScale * heightMap[i][j];
			v[i * mapSize * 3 + j * 3 + 2] = cellSize * (j - mapSize / 2);
			t[i * mapSize * 2 + j * 2]     = i * TextureBit;
			t[i * mapSize * 2 + j * 2 + 1] = j * TextureBit;
		}
	}

	int idx = 0;
	for (int i = 0; i < mapSize - 1; i++) {
		for (int j = 0; j < mapSize - 1; j++) { 
			indices[idx++] = i * mapSize + j;
			indices[idx++] = (i + 1) * mapSize + j;
			indices[idx++] = i * mapSize + j + 1;
			indices[idx++] = (i + 1) * mapSize + j + 1;
		}
	}

	glVertexPointer(3, GL_FLOAT, 0, v);
	glTexCoordPointer(2, GL_FLOAT, 0, t);
}

void Terrain::draw ()
{
	glDrawElements(GL_TRIANGLE_STRIP, (mapSize - 1) * (mapSize - 1) * 4, GL_UNSIGNED_INT, indices);
}

inline float Terrain::noise(int x, int y)
{
        int n = x + y * 57;
        n = (n << 13) ^ n;
        return ( 1.0 - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);    
}

float Terrain::smoothNoise(int x, int y)
{
    float corners = ( noise(x - 1, y - 1) + noise(x + 1, y- 1 ) + noise(x - 1, y + 1) + noise(x + 1, y + 1) ) / 16;
    float sides   = ( noise(x - 1, y) + noise(x + 1, y) + noise(x, y - 1) + noise(x, y + 1) ) / 8;
    float center  =  noise(x, y) / 4;
    return corners + sides + center;
}
