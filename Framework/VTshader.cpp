#include "VTshader.h"
#include "shader.hpp"

bool VTshaderuniform::Init()
{
	shader = LoadShaders("shader/vt_vert.glsl","shader/vt_fragment.glsl");

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

void VTshaderuniform::SetWVP(const glm::mat4 &WVP)
{
	glUniformMatrix4fv(m_gWVPLocation, 1, GL_FALSE, &WVP[0][0]);
}

void VTshaderuniform::SetScale(const glm::vec4 &scale)
{
	glUniform4f(m_scaleLocation, scale.x, scale.y, scale.z, scale.w);
}

void VTshaderuniform::SetOffset(const glm::vec4 &offset)
{
	glUniform4f(m_offsetLocation, offset.x, offset.y, offset.z, offset.w);
}

void VTshaderuniform::SetCurrentPosition(const glm::vec4 &currentPos)
{
	glUniform4f(m_currentPosLocation, currentPos.x, currentPos.y, currentPos.z, currentPos.w);
}

void VTshaderuniform::SetHeightmap(unsigned int TextureUnit)
{
	glUniform1i(m_heightmapLocation, TextureUnit);
}

void VTshaderuniform::SetVtex(unsigned int TextureUnit)
{
	glUniform1i(m_vtexLocation, TextureUnit);
}