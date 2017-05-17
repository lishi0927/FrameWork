#include "shadershadow.h"
#include "shader.hpp"

bool ShaderShadow::Init()
{
	shader = LoadShaders("shader/shadow_vert.glsl", "shader/shadow_fragment.glsl");
	
	m_gWVPLocation = glGetUniformLocation(shader, "gWVP");
	m_offsetLocation = glGetUniformLocation(shader, "offset");
	m_scaleLocation = glGetUniformLocation(shader, "scale");
	m_currentPosLocation = glGetUniformLocation(shader, "currentPos");
	m_vtexLocation = glGetUniformLocation(shader, "vtex");
	m_heightmapLocation = glGetUniformLocation(shader, "texHeightmap");

	if (m_gWVPLocation == 0xFFFFFFFF ||
		m_offsetLocation == 0xFFFFFFFF ||
		m_scaleLocation == 0xFFFFFFFF ||
		m_currentPosLocation == 0xFFFFFFFF ||
		m_vtexLocation == 0xFFFFFFFF ||
		m_heightmapLocation == 0xFFFFFFFF)
		return false;

	return true;
}

void ShaderShadow::SetWVP(const glm::mat4 &WVP)
{
	glUniformMatrix4fv(m_gWVPLocation, 1, GL_TRUE, &WVP[0][0]);
}

void ShaderShadow::SetScale(const glm::vec4 &scale)
{
	glUniform4f(m_scaleLocation, scale.x, scale.y, scale.z, scale.w);
}

void ShaderShadow::SetOffset(const glm::vec4 &offset)
{
	glUniform4f(m_offsetLocation, offset.x, offset.y, offset.z, offset.w);
}

void ShaderShadow::SetCurrentPosition(const glm::vec4 &currentPos)
{
	glUniform4f(m_currentPosLocation, currentPos.x, currentPos.y, currentPos.z, currentPos.w);
}

void ShaderShadow::SetHeightmap(unsigned int TextureUnit)
{
	glUniform1i(m_heightmapLocation, TextureUnit);
}

void ShaderShadow::SetVtex(unsigned int TextureUnit)
{
	glUniform1i(m_vtexLocation, TextureUnit);
}

