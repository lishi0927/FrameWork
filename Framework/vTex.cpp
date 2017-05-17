#include "vTex.h"

VTex::VTex() {
}


VTex::~VTex() {
	for (auto ptex : ptexes)
		delete ptex;
}

void VTex::init(int pageSize, int virtualWidth, bool isHeightmap, int maxLevel) {
	this->pageSize = pageSize;
	this->virtualWidth = virtualWidth;

	ptable.clear();
	w.clear();
	h.clear();
	latest.clear();
	length.clear();
	int tmp;
	for (int i = 0; i < maxLevel; i++) {
		length.push_back((pageSize * virtualWidth) >> i);
		tmp = ceil(length.back() / (float)pageSize - 1e-6);
		tmp = (tmp <= 0) ? 1 : tmp;
		w.push_back(tmp);
		h.push_back(tmp);
		ptable.push_back(new float[w[i] * h[i] * 4]);
		latest.push_back(new bool[w[i] * h[i]]);
	}

	this->maxLevel = maxLevel;
	this->isHeightmap = isHeightmap;


	ptexes.clear();
	PTex *ptex;
	if (isHeightmap) {
		ptex = new PTex(pageSize, virtualWidth);
		ptex->init(true, false, "terrain");
		ptexes.push_back(ptex);
		ptex = new PTex(pageSize, virtualWidth);
		ptex->init(false, true, "AO");
		ptexes.push_back(ptex);
	}
	else {
		ptex = new PTex(pageSize, virtualWidth);
		ptex->init(false, false,"blendTex.dds");
		ptexes.push_back(ptex);
		ptex = new PTex(pageSize, virtualWidth);
		ptex->init(false, false,"blendTex2.dds");
		ptexes.push_back(ptex);
	}

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	if (isHeightmap)
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);
	else {
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, maxLevel - 1);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	if (isHeightmap)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	else
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, w[0], h[0], 0,
		GL_RGBA, GL_FLOAT, nullptr);

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}



void VTex::update(int k, int x, int y) {
	if (latest[k][y * w[k] + x])
		return;
	Page *tp = ptexes[0]->findPage(k, x, y);
	if (tp != nullptr) {
		tp->setUsed();
	}
	else {
		tp = ptexes[0]->getReplacePage();
		for (auto t : ptexes)
			t->update(k, x, y, tp->getX(), tp->getY(),
				w[k], h[k]);
		tp->setUsed();
		ptexes[0]->insert(k, x, y, tp);
	}

	float *pt = ptable[k] + y * w[k] * 4 + x * 4;
	pt[0] = (float)length[k] / ptexes[0]->getPixelSize();
	pt[1] = 0;
	pt[2] = (float)tp->getX() / ptexes[0]->getPhysicalWidth() -
		(float)x / w[k] * pt[0];
	pt[2] += ((float)ptexes[0]->getBorderSize())
		/ ptexes[0]->getPixelSize();
	pt[3] = (float)tp->getY() / ptexes[0]->getPhysicalWidth() -
		(float)y / h[k] * pt[0];
	pt[3] += ((float)ptexes[0]->getBorderSize())
		/ ptexes[0]->getPixelSize();
	latest[k][y * w[k] + x] = true;
}

void VTex::update(int level, float maxScale, float currentX, float currentY) {

	float lx, rx, ly, ry;
	float r = VIEWCHUNKNUMBER;
	r /= CHUNKNUMBER;
	lx = currentX - r / 2;
	rx = currentX + r / 2;
	ly = currentY - r / 2;
	ry = currentY + r / 2;
	int plx, ply, prx, pry;
	int mk = Min(level, maxLevel);
	for (int k = 0; k < mk; k++) {
		plx = floor(lx * w[k] + 1e-6);
		prx = floor(rx * w[k] + 1e-6);
		ply = floor(ly * h[k] + 1e-6);
		pry = floor(ry * h[k] + 1e-6);
		clamp(plx, 0, w[k] - 1);
		clamp(prx, 0, w[k] - 1);
		clamp(ply, 0, h[k] - 1);
		clamp(pry, 0, h[k] - 1);

		for (int x = plx; x <= prx; x++)
			for (int y = ply; y <= pry; y++) {
				update(k, x, y);
			}
	}
}

GLuint VTex::generateTex() {
	glBindTexture(GL_TEXTURE_2D, tex);

	for (int i = 0; i < maxLevel; i++)
		glTexImage2D(GL_TEXTURE_2D, i, GL_RGBA32F_ARB, w[i], h[i], 0,
			GL_RGBA, GL_FLOAT, ptable[i]);

	glBindTexture(GL_TEXTURE_2D, 0);
	return tex;
}

GLuint VTex::getTex() {
	return tex;
}

void VTex::clear() {
	ptexes[0]->clearUsed();
	for (int i = 0; i < maxLevel; i++)
		memset(latest[i], 0, w[i] * h[i]);
}


vector<PTex*> VTex::getPtexes() {
	return ptexes;
}

int VTex::getMaxLevel() const {
	return maxLevel;
}
