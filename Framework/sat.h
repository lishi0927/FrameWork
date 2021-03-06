#pragma once
#include "util.h"

class SAT {
private:
	double maxHeight[CHUNKNUMBER][CHUNKNUMBER];
	double minHeight[CHUNKNUMBER][CHUNKNUMBER];
	double* minSAT[CHUNKNUMBER][CHUNKNUMBER];
	double* maxSAT[CHUNKNUMBER][CHUNKNUMBER];
	int w, h, htop, hoff, ep;
	double restore(double x, int ep, int htop, int hoff, int tot, bool isMin);
	double getVal(double *data, int x, int y);
	double getArea(double *data, int lx, int ly, int rx, int ry, int &num);
	double getMaxOrMin(int lx, int ly, int rx, int ry, bool isMin);
	double getMax(int lx, int ly, int rx, int ry);
	double getMin(int lx, int ly, int rx, int ry);

public:
	SAT();
	~SAT();
	void loadSATData(int x, int y);
	void loadMaxAndMin();
	double getMaxDiff(int lx, int ly, int rx, int ry);
	void loadSATData(float currentX, float currentZ);
};