#ifndef SHADER_UNIFORM
#define SHADER_UNIFORM
#include "GL/glew.h"
#include <vector>

class ShaderUniform
{
public:
	ShaderUniform()
	{
		childindex.clear();
	}

	~ShaderUniform()
	{
		if (block_index != NULL) delete block_index;
	}

	void Enable()
	{
		glUseProgram(shader);
	}

	virtual bool Init() = 0;

	bool* block_index;
	std::vector<int> childindex;
	GLuint shader;
};

#endif
