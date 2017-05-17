#ifndef SHADOW_SHADER_H
#define SHADOW_SHADER_H
#include "shaderuniform.h"
#include <glm/glm.hpp>

class ShaderShadow :public ShaderUniform {
public:
	virtual bool Init();

	void SetWVP(const glm::mat4 &WVP);

	void SetScale(const glm::vec4 &scale);

	void SetOffset(const glm::vec4 &offset);

	void SetCurrentPosition(const glm::vec4 &currentPos);

	void SetHeightmap(unsigned int TextureUnit);

	void SetVtex(unsigned int TextureUnit);

private:
	GLuint m_vtexLocation;
	GLuint m_heightmapLocation;
	GLuint m_scaleLocation;
	GLuint m_offsetLocation;
	GLuint m_currentPosLocation;
	GLuint m_gWVPLocation;
};


#endif
