#pragma once

#include <iostream>
#include <GL/glew.h>
#include "page.h"
#include "pTex.h"
#include <vector>

using namespace std;

class VTex {
private:
	vector<float*> ptable;
	vector<bool*> latest;
	vector<int> w, h;
	vector<int> length;
	GLuint tex;
	vector<PTex*> ptexes;
	int pageSize, virtualWidth;
	int maxLevel;
	bool isHeightmap;

public:
	VTex();
	~VTex();
	void init(int pageSize, int virtualWidth, bool isHeightmap, int maxLevel);
	void update(int k, int x, int y);
	void update(int level,
		float maxScale, float currentX, float currentY);
	GLuint generateTex();
	GLuint getTex();
	void clear();
	//void fillMissData();
	vector<PTex*> getPtexes();
	int getMaxLevel() const;
};