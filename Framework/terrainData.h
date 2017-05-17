#pragma once
#include "util.h"

class TerrainData {
private:
	int version;
	int width, height;
	int ntex, texnamesize;
	int dw, dh;
	unsigned char rt, rt2, rt3, rt4;
	float spacing;
	float* heightmap;
	float* AOmap;
	float maxHeight, minHeight;
	int normalW, normalH;
	unsigned char* normal, *tangent;
	//vector<char*> texnames;
public:
	TerrainData();
	~TerrainData();
	void loadHeightmap(int tx, int ty, int pos);
	void loadAOmap(int tx, int ty, int pos);
	int getWidth() const;
	int getHeight() const;
	float* getHeightmap() const;
	float* getAOmap() const;
	float getMaxHeight() const;
	float getMinHeight() const;
	void output(const char* filename);
	void calNormal();
	unsigned char *getNormal();
	unsigned char *getTangent();
	int getNormalW() const;
	int getNormalH() const;

	void HeightborderData(int tx, int ty, int nSize, int pos);
	void AOborderData(int tx, int ty, int nSize, int pos);
};



