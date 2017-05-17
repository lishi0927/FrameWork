#include "terrainData.h"
#include <string>
#include <glm/glm.hpp>
#include <SOIL/SOIL.h>

using namespace std;

TerrainData::TerrainData() {
	int s = CHUNKSIZE + 2 * HBORDERSIZE;
	width = height = s;
	heightmap = new float[width * height];
	AOmap = new float[width * height];
	for (int i = 0; i < height;i++)
	{
		for (int j = 0; j < width; j++)
		{
			AOmap[i * width + j] = 0;
		}
	}

	normalW = normalH = s;
	normal = new unsigned char[4 * normalW * normalH];
	tangent = new unsigned char[4 * normalW * normalH];
}


TerrainData::~TerrainData() {
	if (heightmap)
		delete heightmap;
	if (AOmap)
		delete AOmap;
	if (normal)
		delete normal;
	if (tangent)
		delete tangent;
}

void TerrainData::loadAOmap(int tx, int ty, int pos)
{
	if (pos != CENTER) {
		AOborderData(tx, ty, HBORDERSIZE, pos);
		return;
	}

	string filename = "data/" + getChunkName(tx, ty) + "/AO";
	FILE *f = NULL;
	fopen_s(&f, filename.c_str(), "r");

	for (int i = 0; i< CHUNKSIZE; i++)
		fread(AOmap + (i + BORDERSIZE) * width + BORDERSIZE, CHUNKSIZE * sizeof(float), 1, f);

	fclose(f);
}

void TerrainData::loadHeightmap(int tx, int ty, int pos) {
	if (pos != CENTER) {
		HeightborderData(tx, ty, HBORDERSIZE, pos);
		return;
	}

	string filename = "data/" + getChunkName(tx, ty) + "/terrain";
	FILE *f;
	openfile(filename.c_str(), f);

	unsigned char header[257];
	fread(header, 256, 1, f);

	int w, h;

	version = (int)header[0];
	w = (int)header[4];
	h = (int)header[8];
	spacing = (float)header[12];
	ntex = (int)header[16];
	texnamesize = (int)header[20];
	dw = (int)header[24];
	dh = (int)header[28];
	rt = (unsigned char)header[32];
	rt2 = (unsigned char)header[33];
	rt3 = (unsigned char)header[34];
	rt4 = (unsigned char)header[35];

	assert(width == height);
	assert(width > 0);
	assert(ntex > 0);
	assert(texnamesize == 128);

	char *texname = new char[texnamesize];
	for (int i = 0; i < ntex; i++) {
		fread(texname, texnamesize, 1, f);
	}
	delete texname;

	//printf("%d %d\n", width, height);

	//heightmap = new float[(width - 2) * (height - 2)];

	//fread(heightmap + HBORDERSIZE * width + HBORDERSIZE, w * sizeof(float),
	//	1, f);
	for (int i = 0; i < h; i++) {
		if (i == 0 || i == h - 1) {
			continue;
		}
		float tmp;
		fread(&tmp, 4, 1, f);
		fread(heightmap + (i + HBORDERSIZE-1) * width + HBORDERSIZE,
			sizeof(float) * (w - 2), 1, f);
		fread(&tmp, 4, 1, f);
	}
	fclose(f);
}

int TerrainData::getWidth() const {
	return width;
}

int TerrainData::getHeight() const {
	return height;
}

float* TerrainData::getHeightmap() const {
	return heightmap;
}

float* TerrainData::getAOmap() const {
	return AOmap;
}

float TerrainData::getMaxHeight() const {
	return maxHeight;
}

float TerrainData::getMinHeight() const {
	return minHeight;
}

void TerrainData::output(const char * filename) {
	if (!AOmap)
		return;
//	FILE*f = fopen(filename, "w");
//	for (int i = 0; i < height; i++) {
//		for (int j = 0; j < width; j++) {
//			fprintf(f,"%f ",AOmap[i * width + j]);
//		}
//		fprintf(f,"\n");
//	}
//	fclose(f);
	//freopen("CON", "w", stdout);
	unsigned char* result = new unsigned char[width * height];
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			result[i * width + j] = AOmap[i * width + j] * 255;
		}
	}
	SOIL_save_image(filename, SOIL_SAVE_TYPE_BMP, width, height, 1, result);
	delete result;
}

void TerrainData::calNormal() {
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++) {
			if (i == 0 || j == 0 || i == width - 1 || j == height - 1)
			{
				normal[i * normalW * 4 + j * 4] = 128;
				normal[i * normalW * 4 + j * 4 + 1] = 255;
				normal[i * normalW * 4 + j * 4 + 2] = 128;
				normal[i * normalW * 4 + j * 4 + 3] = 255;
				continue;
			}
			glm::vec3 va, vb, nm;
			va = glm::vec3(0,  heightmap[(i + 1) * width + j] -
				heightmap[(i - 1) * width + j],2);
			vb = glm::vec3(2, heightmap[i * width + j + 1] -
				heightmap[i * width + j - 1], 0);
			//va.z = vb.z = 0;
			nm = glm::normalize(glm::cross(vb, va));
			nm = glm::vec3((nm.x+1.0)/2.0, (nm.y + 1.0) / 2.0, (nm.z + 1.0) / 2.0);
			normal[i * normalW * 4 + j * 4] = nm.x * 255;
			normal[i * normalW * 4 + j * 4 + 1] = nm.y * 255;
			normal[i * normalW * 4 + j * 4 + 2] = nm.z * 255;
			normal[i * normalW * 4 + j * 4 + 3] = 255;

		//	vb = glm::normalize(vb);
		//	tangent[(i - 1) * normalW * 4 + (j - 1) * 4] = vb.x * 255;
		//	tangent[(i - 1) * normalW * 4 + (j - 1) * 4 + 1] = vb.y * 255;
		//	tangent[(i - 1) * normalW * 4 + (j - 1) * 4 + 2] = vb.z * 255;
		//	tangent[(i - 1) * normalW * 4 + (j - 1) * 4 + 3] = 255;
		}
}

unsigned char * TerrainData::getNormal() {
	return normal;
}

unsigned char * TerrainData::getTangent() {
	return tangent;
}

int TerrainData::getNormalW() const {
	return normalW;
}

int TerrainData::getNormalH() const {
	return normalH;
}

void TerrainData::HeightborderData(int tx, int ty, int nSize, int pos) {
	assert(nSize == HBORDERSIZE);
	string filename;
	filename = "data/" + getChunkName(tx, ty) + "/terrain";

	FILE *f;
	openfile(filename.c_str(), f);

	unsigned char header[30];
	fread(header, 24, 1, f);
	int w, h;
	w = (int)header[4];
	h = (int)header[8];
	ntex = (int)header[16];
	texnamesize = (int)header[20];

	int offset = 256 + ntex * texnamesize;

	float *hp;
	if (pos > CENTER) {
		//width = height = nSize;
		for (int i = 0; i < nSize; i++) {
			switch (pos) {
			case LEFTTOP:
				seekfile(f, offset + (i + 1) * w * sizeof(float) + 4, SEEK_SET);
				hp = heightmap + (i + HBORDERSIZE + CHUNKSIZE + 1) * width
					- HBORDERSIZE;
				break;
			case RIGHTTOP:
				seekfile(f, offset + (i + 2) * w * sizeof(float) -
					(nSize + 1) * sizeof(float), SEEK_SET);
				hp = heightmap + (i + HBORDERSIZE + CHUNKSIZE) * width;
				break;
			case LEFTBOTTOM:
				seekfile(f, offset + (h - nSize - 1 + i) * w *
					sizeof(float) + 4, SEEK_SET);
				hp = heightmap + (i + 1) * width - HBORDERSIZE;
				break;
			case RIGHTBOTTOM:
				seekfile(f, offset + (h - nSize + i) * w
					* sizeof(float) - (nSize + 1) * sizeof(float), SEEK_SET);
				hp = heightmap + i * width;
				break;
			}
			fread(hp, nSize * sizeof(float), 1, f);
		}
	}
	else {
		if (pos == TOP || pos == BOTTOM) {
			//width = w - 2;
			//height = nSize;
			for (int i = 0; i < nSize; i++) {
				if (pos == TOP) {
					seekfile(f, offset + (i + 1) * w * sizeof(float) + 4,
						SEEK_SET);
					hp = heightmap + (i + HBORDERSIZE + CHUNKSIZE) * width
						+ HBORDERSIZE;
				}
				else {
					seekfile(f, offset + (h - nSize + i - 1) * w *
						sizeof(float) + 4, SEEK_SET);
					hp = heightmap + i * width + HBORDERSIZE;
				}
				fread(hp, (w - 2) * sizeof(float), 1, f);
			}
		}
		else {
			//width = nSize;
			//height = h - 2;
			for (int i = 0; i < h; i++) {
				if (i == 0 || i == h - 1)
					continue;
				if (pos == LEFT) {
					seekfile(f, offset + (i * w + 1) * sizeof(float), SEEK_SET);
					hp = heightmap + (i + HBORDERSIZE) * width - HBORDERSIZE;
				}
				else {
					seekfile(f, offset + (i + 1) * w * sizeof(float) -
						(nSize + 1) * sizeof(float), SEEK_SET);
					hp = heightmap + (i - 1 + HBORDERSIZE) * width;
				}
				fread(hp, HBORDERSIZE * sizeof(float), 1, f);
			}
		}
	}
	fclose(f);
}

void TerrainData::AOborderData(int tx, int ty, int nSize, int pos)
{
	assert(nSize == HBORDERSIZE);
	string filename;
	filename = "data/" + getChunkName(tx, ty) + "/AO";
	
	FILE *f = NULL;
	fopen_s(&f, filename.c_str(), "r");
	
	float *aop = NULL;
	if (pos > CENTER) {
		for (int i = 0; i < nSize; i++) {
			switch (pos) {
			case LEFTTOP:
				fseek(f, i * CHUNKSIZE, SEEK_SET);
				aop = AOmap + (i + HBORDERSIZE + CHUNKSIZE + 1) * width - HBORDERSIZE;
				break;
			case RIGHTTOP:
				fseek(f, (i + 1) * CHUNKSIZE - nSize, SEEK_SET);
				aop = AOmap + (i + HBORDERSIZE + CHUNKSIZE) * width;
				break;
			case LEFTBOTTOM:
				fseek(f, (CHUNKSIZE - nSize + i) * CHUNKSIZE, SEEK_SET);
				aop = AOmap + (i + 1) * width - HBORDERSIZE;
				break;
			case RIGHTBOTTOM:
				fseek(f, (CHUNKSIZE - nSize + i + 1) * CHUNKSIZE - nSize, SEEK_SET);
				aop = AOmap + i * width;
				break;
			}
			fread(aop, nSize * sizeof(float), 1, f);
		}
	}
	else {
		if (pos == TOP || pos == BOTTOM) {
			
			for (int i = 0; i < nSize; i++) {
				if (pos == TOP) {
					fseek(f, i * CHUNKSIZE, SEEK_SET);
					aop = AOmap + (i + HBORDERSIZE + CHUNKSIZE) * width + HBORDERSIZE;
				}
				else {
					fseek(f, (CHUNKSIZE - nSize + i) * CHUNKSIZE, SEEK_SET);
					aop = AOmap + i * width + HBORDERSIZE;
				}
				fread(aop, CHUNKSIZE * sizeof(float), 1, f);
			}
		}
		else {
			//width = nSize;
			//height = h - 2;
			for (int i = 0; i < CHUNKSIZE; i++)
			{
				if (pos == LEFT)
				{
					fseek(f, i * CHUNKSIZE, SEEK_SET);
					aop = AOmap + (i + 1 + HBORDERSIZE) * width - HBORDERSIZE;
				}
				else
				{
					fseek(f, (i + 1) * CHUNKSIZE - nSize, SEEK_SET);
					aop = AOmap + (i + HBORDERSIZE) * width;
				}
				fread(aop, HBORDERSIZE * sizeof(float), 1, f);
			}
		}
	}
	fclose(f);
}
