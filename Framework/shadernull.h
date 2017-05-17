#ifndef SHADER_NULL
#define SHADER_NULL

#include "shaderuniform.h"
#include "light.h"

class Shader_NULL : public ShaderUniform {
public:


	virtual bool Init();

	void SetWVP(const glm::mat4 &WVP);

	void SetScale(const glm::vec4 &scale);

	void SetOffset(const glm::vec4 &offset);

	void SetCurrentPosition(const glm::vec4 &currentPos);

	void SetHeightmap(unsigned int TextureUnit);

	void SetVtex(unsigned int TextureUnit);

	void SetBtex(unsigned int TextureUnit);

	void SetBlendtex(int index, unsigned int TextureUnit);

	void SetColortex(int index, unsigned int TextureUnit);
private:
	GLuint m_WVPLocation;

	GLuint m_vtexLocation;
	GLuint m_heightmapLocation;
	GLuint m_scaleLocation;
	GLuint m_offsetLocation;
	GLuint m_currentPosLocation;

	GLuint m_texLocation[6];
	GLuint m_bexLocation;
	GLuint m_bexBlendlocation[2];
};


#endif
