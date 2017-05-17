#ifndef SHADING_H
#define SHADING_H

class ShadingLOD {
public:
	ShadingLOD()
	{
		BRDF = 0;
		heightmap = 0;
	}

	ShadingLOD(int tbrdf, int theightmap)
	{
		BRDF = tbrdf;
		heightmap = theightmap;
	}

	int BRDF;
	int heightmap;
};

#endif
