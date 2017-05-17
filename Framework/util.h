#ifndef UTILS_H
#define UTILS_H
#include <vector>
#include <cassert>
#include <string>
#include <glm/glm.hpp>

#define CHUNKNUMBER (32)
#define CHUNKSIZE (51)
#define VIEWCHUNKNUMBER (20)

#define BLENDSIZE (128)
#define LEVELOFBLENDTEX (9)

#define XMIN (15)
#define XMAX (64)
#define YMIN (-24)
#define YMAX (25)

#define LEVEL (5)

#define Min(a, b) ((a) < (b) ? (a) : (b))
#define Max(a, b) ((a) > (b) ? (a) : (b))

#define RIGHT (0)
#define LEFT (1)
#define BOTTOM (2)
#define TOP (3)
#define CENTER (4)
#define RIGHTBOTTOM (5)
#define RIGHTTOP (6)
#define LEFTBOTTOM (7)
#define LEFTTOP (8)

#define BORDERSIZE (4)
#define HBORDERSIZE (4)

//const int GRID = CHUNKSIZE * VIEWCHUNKNUMBER;
const int GRID = CHUNKSIZE;
const float MAXSCALE = 100.0f * VIEWCHUNKNUMBER / 4.0f;
const int WIDTH = 1024, HEIGHT = 720;
const int FEEDBACK_WIDTH = WIDTH / 10;
const int FEEDBACK_HEIGHT = HEIGHT / 10;
const float fov = 45.0f;
const float znear = 0.1f;
const float zfar = 2000.0f;

using namespace std;

string getChunkName(int x, int y);

void clamp(int &x, int l, int r);

float clampf(float x, float a, float b);

void openfile(const char* filename, FILE* &fp);

void seekfile(FILE* fp, int off, int pos);

glm::mat4 PerspectiveProjection(float fov, float ar, float near, float far);

glm::mat4 OrthoProjection(float l, float r, float b, float t, float n, float f);

glm::mat4 ViewProjection(glm::vec3 eye, glm::vec3 center, glm::vec3 up);
#endif