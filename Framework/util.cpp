#include "util.h"

string getChunkName(int x, int y) {
	x += XMIN;
	y += YMIN;
	char s[20];
	string ret("");
	string tmp;
	sprintf(s, "%04x", x);
	tmp = s;
	if (x < 0)
		ret += tmp.substr(4, 4);
	else
		ret += tmp;
	sprintf(s, "%04x", y);
	tmp = s;
	if (y < 0)
		ret += tmp.substr(4, 4);
	else
		ret += tmp;
	return ret;
}

void clamp(int &x, int l, int r) {
	assert(l <= r);
	x = (x < l) ? l : x;
	x = (x > r) ? r : x;
}

float clampf(float x, float a, float b)
{
	if (x > b)return b;
	else if (x < a) return a;
	else return x;
}

void openfile(const char* filename, FILE* &fp) {
	if ((fp = fopen(filename, "rb")) == nullptr) {
		printf("Cannot open file.\n");
		//exit(1);
	}
}

void seekfile(FILE* fp, int off, int pos) {
	assert(pos >= 0 && pos <= 2);
	if (fseek(fp, off, 0)) {
		printf("file seek failed.\n");
		exit(1);
	}
}

glm::mat4 PerspectiveProjection(float fov, float ar, float near, float far)
{
	glm::mat4 projection = glm::mat4(1.0 / tan(glm::radians(fov / 2)) / ((float)WIDTH / (float)HEIGHT), 0.0, 0.0, 0.0,
			0.0, 1.0 / tan(glm::radians(fov / 2)), 0.0, 0.0,
			0.0, 0.0, (znear + zfar) / (zfar - znear), -2.0* znear * zfar / (zfar - znear),
				0.0, 0.0, 1.0, 0.0);

	return glm::transpose(projection);
}

glm::mat4 OrthoProjection(float l, float r, float b, float t, float n, float f)
{
	glm::mat4 projection = glm::mat4(2.0f / (r - l), 0.0, 0.0, -(r + l) / (r - l),
		0.0, 2.0f / (t - b), 0.0, -(t + b) / (t - b),
		0.0, 0.0, 2.0f / (f - n), -(f + n) / (f - n),
		0.0, 0.0, 0.0, 1.0);
	return glm::transpose(projection);
}

glm::mat4 ViewProjection(glm::vec3 eye, glm::vec3 center, glm::vec3 up)
{
	glm::vec3 Z = center - eye;
	Z = glm::normalize(Z);
	glm::vec3 Y = up;
	glm::vec3 X = glm::cross(Y, Z);
	Y = glm::cross(Z, X);
	X = glm::normalize(X);
	Y = glm::normalize(Y);
	glm::mat4 viewtest = glm::mat4(X.x, X.y,X.z,0.0,
		Y.x, Y.y, Y.z, 0.0,
		Z.x, Z.y, Z.z,0.0,
		0, 0, 0, 1.0);
	viewtest = glm::transpose(viewtest);
	glm::mat4 viewtranslate = glm::mat4(1.0, 0.0, 0.0, -eye.x,
		0.0, 1.0, 0.0, -eye.y,
		0.0, 0.0, 1.0, -eye.z,
		0.0, 0.0, 0.0, 1.0);
	viewtranslate = glm::transpose(viewtranslate);
	return viewtest * viewtranslate;
}