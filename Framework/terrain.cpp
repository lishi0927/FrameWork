#include "terrain.h"

void Block::draw()
{

}

void Block::VertexInit()
{
	for (int j = 0; j < GRID + 1; j++)
		for (int i = 0; i < GRID + 2; i++) {

			for (int k = 0; k < ((i == 0) ? 2 : 1); k++) {
				//			for (int k = 0; k < 1; k++) {
				vertices.push_back((float)i / GRID);
				vertices.push_back(0);
				vertices.push_back((float)j / GRID);
				//                printf("%d, %d, %d\n", i, 0, j);
			}

			j++;
			for (int k = 0; k < ((i == GRID + 1) ? 2 : 1); k++) {
				//		for (int k = 0; k < 1; k++) {
				vertices.push_back((float)i / GRID);
				vertices.push_back(0);
				vertices.push_back((float)j / GRID);
				//                printf("%d, %d, %d\n", i, 0, j);
			}
			j--;
		}
	for (int i = 0; i < 4; i++) {
		int tmp;
		tmp = (i < 2) ? 1 : 0;
		for (int j = 0; j < 2; j++) {
			vertices.push_back((float)(GRID + tmp) / GRID);
			vertices.push_back(0);
			vertices.push_back((float)GRID / GRID);
		}
	}

	//T-junction removal
	float x, y;
	x = y = GRID;
	for (int i = 0; i < GRID * 2; x -= 0.5, i++) {
		vertices.push_back((float)x / GRID);
		vertices.push_back(0);
		vertices.push_back((float)y / GRID);
	}
	vertices.push_back((float)x / GRID);
	vertices.push_back(0);
	vertices.push_back((float)y / GRID);
	for (int i = 0; i < GRID * 2; y -= 0.5, i++) {
		vertices.push_back((float)x / GRID);
		vertices.push_back(0);
		vertices.push_back((float)y / GRID);
	}
	vertices.push_back((float)x / GRID);
	vertices.push_back(0);
	vertices.push_back((float)y / GRID);
	for (int i = 0; i < GRID * 2; x += 0.5, i++) {
		vertices.push_back((float)x / GRID);
		vertices.push_back(0);
		vertices.push_back((float)y / GRID);
	}
	vertices.push_back((float)x / GRID);
	vertices.push_back(0);
	vertices.push_back((float)y / GRID);
	for (int i = 0; i < GRID * 2; y += 0.5, i++) {
		vertices.push_back((float)x / GRID);
		vertices.push_back(0);
		vertices.push_back((float)y / GRID);
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat),
		&vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
		(GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}