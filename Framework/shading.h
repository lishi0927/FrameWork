#ifndef SHADING_H
#define SHADING_H

class ShadingLOD {
public:
	ShadingLOD()
	{
		BRDF = 0;
		heightmap = 0;
	}

	int BRDF;
	int heightmap;
};

#endif
