#ifndef TERRAIN_H
#define TERRAIN_H

#include <GL/glew.h>
#include <vector>
#include "shading.h"

#define GRID  1024

class Block {
public:
	Block(int index)
	{
		blockID = index;

		vertices.clear();

		VertexInit();
	}

	~Block()
	{
		vertices.clear();
	}

	void VertexInit();

	void setShadingLOD(ShadingLOD lod)
	{
		block_lod = lod;
		
	}

	void draw();

	int blockID;	
private:
	std::vector<GLfloat> vertices;
	ShadingLOD block_lod;
	GLuint vao;
	GLuint vbo;
	
};

#endif