#include "shadernull.h"
#include "shader.hpp"

bool Shader_NULL::Init()
{
	shader = LoadShaders("shader/null_vert.glsl", "shader/null_fragment.glsl");

	m_WVPLocation = glGetUniformLocation(shader, "gWVP");
	m_offsetLocation = glGetUniformLocation(shader, "offset");
	m_scaleLocation = glGetUniformLocation(shader, "scale");
	m_currentPosLocation = glGetUniformLocation(shader, "currentPos");
	m_vtexLocation = glGetUniformLocation(shader, "vtex");
	m_heightmapLocation = glGetUniformLocation(shader, "texHeightmap");
	m_bexLocation = glGetUniformLocation(shader, "btex");

	for (int i = 0; i < 2; i++)
	{
		char Name[128];
		memset(Name, 0, sizeof(Name));
		SNPRINTF(Name, sizeof(Name), "blendtex%d", i);
		m_bexBlendlocation[i] = glGetUniformLocation(shader, Name);
		if (m_bexBlendlocation[i] == 0xFFFFFFFF) return false;
	}

	for (int i = 0; i < 6; i++)
	{
		char Name[128];
		memset(Name, 0, sizeof(Name));
		SNPRINTF(Name, sizeof(Name), "tex%d", i);
		m_texLocation[i] = glGetUniformLocation(shader, Name);
		if (m_texLocation[i] == 0xFFFFFFFF) return false;
	}

	if (m_WVPLocation == 0xFFFFFFFF ||
		m_offsetLocation == 0xFFFFFFFF ||
		m_scaleLocation == 0xFFFFFFFF ||
		m_currentPosLocation == 0xFFFFFFFF ||
		m_vtexLocation == 0xFFFFFFFF ||
		m_heightmapLocation == 0xFFFFFFFF ||
		m_bexLocation == 0xFFFFFFFF) {
		return false;
	}

	return true;
}

void Shader_NULL::SetWVP(const glm::mat4 &WVP)
{
	glUniformMatrix4fv(m_WVPLocation, 1, GL_FALSE, &WVP[0][0]);
}

void Shader_NULL::SetScale(const glm::vec4 &scale)
{
	glUniform4f(m_scaleLocation, scale.x, scale.y, scale.z, scale.w);
}

void Shader_NULL::SetOffset(const glm::vec4 &offset)
{
	glUniform4f(m_offsetLocation, offset.x, offset.y, offset.z, offset.w);
}

void Shader_NULL::SetCurrentPosition(const glm::vec4 &currentPos)
{
	glUniform4f(m_currentPosLocation, currentPos.x, currentPos.y, currentPos.z, currentPos.w);
}

void Shader_NULL::SetHeightmap(unsigned int TextureUnit)
{
	glUniform1i(m_heightmapLocation, TextureUnit);
}

void Shader_NULL::SetVtex(unsigned int TextureUnit)
{
	glUniform1i(m_vtexLocation, TextureUnit);
}

void Shader_NULL::SetBtex(unsigned int TextureUnit)
{
	glUniform1i(m_bexLocation, TextureUnit);
}

void Shader_NULL::SetBlendtex(int index, unsigned int TextureUnit)
{
	glUniform1i(m_bexBlendlocation[index], TextureUnit);
}

void Shader_NULL::SetColortex(int index, unsigned int TextureUnit)
{
	glUniform1i(m_texLocation[index], TextureUnit);
}
