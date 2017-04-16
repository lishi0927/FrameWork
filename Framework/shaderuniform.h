#ifndef SHADER_UNIFORM
#define SHADER_UNIFORM
#include "GL/glew.h"

class ShaderUniform
{
public:
	void Enable()
	{
		glUseProgram(shader);
	}

	virtual bool Init() = 0;

	GLuint shader;
};

#endif
