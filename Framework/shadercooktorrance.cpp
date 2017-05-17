#include "shadercooktorrance.h"
#include "shader.hpp"

bool Shader_CookTorrance::Init()
{
	shader = LoadShaders("shader/cooktorrance_vert.glsl", "shader/cooktorrance_fragment.glsl");

	m_WVPLocation = glGetUniformLocation(shader, "gWVP");
	m_WorldMatrixLocation = glGetUniformLocation(shader, "gWorld");
	m_samplerLocation = glGetUniformLocation(shader, "gSampler");
	m_eyeWorldPosLocation = glGetUniformLocation(shader, "gEyeWorldPos");
	m_dirLightLocation.Color = glGetUniformLocation(shader, "gDirectionalLight.Base.Color");
	m_dirLightLocation.AmbientIntensity = glGetUniformLocation(shader, "gDirectionalLight.Base.AmbientIntensity");
	m_dirLightLocation.Direction = glGetUniformLocation(shader, "gDirectionalLight.Direction");
	m_dirLightLocation.DiffuseIntensity = glGetUniformLocation(shader, "gDirectionalLight.Base.DiffuseIntensity");
	m_matSpecularIntensityLocation = glGetUniformLocation(shader, "gMatSpecularIntensity");
	m_matSpecularPowerLocation = glGetUniformLocation(shader, "gSpecularPower");
	m_numPointLightsLocation = glGetUniformLocation(shader, "gNumPointLights");
	m_numSpotLightsLocation = glGetUniformLocation(shader, "gNumSpotLights");

	m_offsetLocation = glGetUniformLocation(shader, "offset");
	m_scaleLocation = glGetUniformLocation(shader, "scale");
	m_currentPosLocation = glGetUniformLocation(shader, "currentPos");
	m_vtexLocation = glGetUniformLocation(shader, "vtex");
	m_heightmapLocation = glGetUniformLocation(shader, "texHeightmap");
	m_bexLocation = glGetUniformLocation(shader, "btex");

	if (m_dirLightLocation.AmbientIntensity == 0xFFFFFFFF ||
		m_WVPLocation == 0xFFFFFFFF ||
		m_WorldMatrixLocation == 0xFFFFFFFF ||
		m_samplerLocation == 0xFFFFFFFF ||
		m_eyeWorldPosLocation == 0xFFFFFFFF ||
		m_dirLightLocation.Color == 0xFFFFFFFF ||
		m_dirLightLocation.DiffuseIntensity == 0xFFFFFFFF ||
		m_dirLightLocation.Direction == 0xFFFFFFFF ||
		m_matSpecularIntensityLocation == 0xFFFFFFFF ||
		m_matSpecularPowerLocation == 0xFFFFFFFF ||
		m_numPointLightsLocation == 0xFFFFFFFF ||
		m_numSpotLightsLocation == 0xFFFFFFFF ||
		m_offsetLocation == 0xFFFFFFFF ||
		m_scaleLocation == 0xFFFFFFFF ||
		m_currentPosLocation == 0xFFFFFFFF ||
		m_vtexLocation == 0xFFFFFFFF ||
		m_heightmapLocation == 0xFFFFFFFF ||
		m_bexLocation == 0xFFFFFFFF) {
		return false;
	}

	for (int i = 0; i < 9; i++)
	{
		char Name[128];
		memset(Name, 0, sizeof(Name));
		SNPRINTF(Name, sizeof(Name), "tex%d", i);
		m_texLocation[i] = glGetUniformLocation(shader, Name);
		if (m_texLocation[i] == 0xFFFFFFFF) return false;
	}

	for (int i = 0; i < 2; i++)
	{
		char Name[128];
		memset(Name, 0, sizeof(Name));
		SNPRINTF(Name, sizeof(Name), "blendtex%d", i);
		m_bexBlendlocation[i] = glGetUniformLocation(shader, Name);
		if (m_bexBlendlocation[i] == 0xFFFFFFFF) return false;
	}

	for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_pointLightsLocation); i++) {
		char Name[128];
		memset(Name, 0, sizeof(Name));
		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Base.Color", i);
		m_pointLightsLocation[i].Color = glGetUniformLocation(shader, Name);

		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Base.AmbientIntensity", i);
		m_pointLightsLocation[i].AmbientIntensity = glGetUniformLocation(shader, Name);

		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Position", i);
		m_pointLightsLocation[i].Position = glGetUniformLocation(shader, Name);

		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Base.DiffuseIntensity", i);
		m_pointLightsLocation[i].DiffuseIntensity = glGetUniformLocation(shader, Name);

		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Atten.Constant", i);
		m_pointLightsLocation[i].Atten.Constant = glGetUniformLocation(shader, Name);

		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Atten.Linear", i);
		m_pointLightsLocation[i].Atten.Linear = glGetUniformLocation(shader, Name);

		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Atten.Exp", i);
		m_pointLightsLocation[i].Atten.Exp = glGetUniformLocation(shader, Name);

		if (m_pointLightsLocation[i].Color == 0xFFFFFFFF ||
			m_pointLightsLocation[i].AmbientIntensity == 0xFFFFFFFF ||
			m_pointLightsLocation[i].Position == 0xFFFFFFFF ||
			m_pointLightsLocation[i].DiffuseIntensity == 0xFFFFFFFF ||
			m_pointLightsLocation[i].Atten.Constant == 0xFFFFFFFF ||
			m_pointLightsLocation[i].Atten.Linear == 0xFFFFFFFF ||
			m_pointLightsLocation[i].Atten.Exp == 0xFFFFFFFF) {
			return false;
		}
	}

	for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_spotLightsLocation); i++) {
		char Name[128];
		memset(Name, 0, sizeof(Name));
		SNPRINTF(Name, sizeof(Name), "gSpotLights[%d].Base.Base.Color", i);
		m_spotLightsLocation[i].Color = glGetUniformLocation(shader, Name);

		SNPRINTF(Name, sizeof(Name), "gSpotLights[%d].Base.Base.AmbientIntensity", i);
		m_spotLightsLocation[i].AmbientIntensity = glGetUniformLocation(shader, Name);

		SNPRINTF(Name, sizeof(Name), "gSpotLights[%d].Base.Position", i);
		m_spotLightsLocation[i].Position = glGetUniformLocation(shader, Name);

		SNPRINTF(Name, sizeof(Name), "gSpotLights[%d].Direction", i);
		m_spotLightsLocation[i].Direction = glGetUniformLocation(shader, Name);

		SNPRINTF(Name, sizeof(Name), "gSpotLights[%d].Cutoff", i);
		m_spotLightsLocation[i].Cutoff = glGetUniformLocation(shader, Name);

		SNPRINTF(Name, sizeof(Name), "gSpotLights[%d].Base.Base.DiffuseIntensity", i);
		m_spotLightsLocation[i].DiffuseIntensity = glGetUniformLocation(shader, Name);

		SNPRINTF(Name, sizeof(Name), "gSpotLights[%d].Base.Atten.Constant", i);
		m_spotLightsLocation[i].Atten.Constant = glGetUniformLocation(shader, Name);

		SNPRINTF(Name, sizeof(Name), "gSpotLights[%d].Base.Atten.Linear", i);
		m_spotLightsLocation[i].Atten.Linear = glGetUniformLocation(shader, Name);

		SNPRINTF(Name, sizeof(Name), "gSpotLights[%d].Base.Atten.Exp", i);
		m_spotLightsLocation[i].Atten.Exp = glGetUniformLocation(shader, Name);

		if (m_spotLightsLocation[i].Color == 0xFFFFFFFF ||
			m_spotLightsLocation[i].AmbientIntensity == 0xFFFFFFFF ||
			m_spotLightsLocation[i].Position == 0xFFFFFFFF ||
			m_spotLightsLocation[i].Direction == 0xFFFFFFFF ||
			m_spotLightsLocation[i].Cutoff == 0xFFFFFFFF ||
			m_spotLightsLocation[i].DiffuseIntensity == 0xFFFFFFFF ||
			m_spotLightsLocation[i].Atten.Constant == 0xFFFFFFFF ||
			m_spotLightsLocation[i].Atten.Linear == 0xFFFFFFFF ||
			m_spotLightsLocation[i].Atten.Exp == 0xFFFFFFFF) {
			return false;
		}
	}

	return true;
}

void Shader_CookTorrance::SetWVP(const glm::mat4 &WVP)
{
	glUniformMatrix4fv(m_WVPLocation, 1, GL_FALSE, &WVP[0][0]);
}

void Shader_CookTorrance::SetWorldMatrix(const glm::mat4 &WorldMatrix)
{
	glUniformMatrix4fv(m_WorldMatrixLocation, 1, GL_FALSE, &WorldMatrix[0][0]);
}

void Shader_CookTorrance::SetDirectionalLight(const DirectionalLight& Light)
{
	glUniform3f(m_dirLightLocation.Color, Light.Color.x, Light.Color.y, Light.Color.z);
	glUniform1f(m_dirLightLocation.AmbientIntensity, Light.AmbientIntensity);
	glm::vec3 Direction = Light.Direction;
	Direction = glm::normalize(Direction);
	glUniform3f(m_dirLightLocation.Direction, Direction.x, Direction.y, Direction.z);
	glUniform1f(m_dirLightLocation.DiffuseIntensity, Light.DiffuseIntensity);
}

void Shader_CookTorrance::SetEyeWorldPos(const glm::vec3& EyeWorldPos)
{
	glUniform3f(m_eyeWorldPosLocation, EyeWorldPos.x, EyeWorldPos.y, EyeWorldPos.z);
}

void Shader_CookTorrance::SetMatSpecularIntensity(float Intensity)
{
	glUniform1f(m_matSpecularIntensityLocation, Intensity);
}

void Shader_CookTorrance::SetMatSpecularPower(float Power)
{
	glUniform1f(m_matSpecularPowerLocation, Power);
}

void Shader_CookTorrance::SetPointLights(unsigned int NumLights, const PointLight* pLights)
{
	glUniform1i(m_numPointLightsLocation, NumLights);

	for (unsigned int i = 0; i < NumLights; i++) {
		glUniform3f(m_pointLightsLocation[i].Color, pLights[i].Color.x, pLights[i].Color.y, pLights[i].Color.z);
		glUniform1f(m_pointLightsLocation[i].AmbientIntensity, pLights[i].AmbientIntensity);
		glUniform1f(m_pointLightsLocation[i].DiffuseIntensity, pLights[i].DiffuseIntensity);
		glUniform3f(m_pointLightsLocation[i].Position, pLights[i].Position.x, pLights[i].Position.y, pLights[i].Position.z);
		glUniform1f(m_pointLightsLocation[i].Atten.Constant, pLights[i].Attenuation.Constant);
		glUniform1f(m_pointLightsLocation[i].Atten.Linear, pLights[i].Attenuation.Linear);
		glUniform1f(m_pointLightsLocation[i].Atten.Exp, pLights[i].Attenuation.Exp);
	}
}

void Shader_CookTorrance::SetSpotLights(unsigned int NumLights, const SpotLight* pLights)
{
	glUniform1i(m_numSpotLightsLocation, NumLights);

	for (unsigned int i = 0; i < NumLights; i++) {
		glUniform3f(m_spotLightsLocation[i].Color, pLights[i].Color.x, pLights[i].Color.y, pLights[i].Color.z);
		glUniform1f(m_spotLightsLocation[i].AmbientIntensity, pLights[i].AmbientIntensity);
		glUniform1f(m_spotLightsLocation[i].DiffuseIntensity, pLights[i].DiffuseIntensity);
		glUniform3f(m_spotLightsLocation[i].Position, pLights[i].Position.x, pLights[i].Position.y, pLights[i].Position.z);
		glm::vec3 Direction = pLights[i].Direction;
		Direction = glm::normalize(Direction);
		glUniform3f(m_spotLightsLocation[i].Direction, Direction.x, Direction.y, Direction.z);
		glUniform1f(m_spotLightsLocation[i].Cutoff, cosf(glm::radians(pLights[i].Cutoff)));
		glUniform1f(m_spotLightsLocation[i].Atten.Constant, pLights[i].Attenuation.Constant);
		glUniform1f(m_spotLightsLocation[i].Atten.Linear, pLights[i].Attenuation.Linear);
		glUniform1f(m_spotLightsLocation[i].Atten.Exp, pLights[i].Attenuation.Exp);
	}
}

void Shader_CookTorrance::SetScale(const glm::vec4 &scale)
{
	glUniform4f(m_scaleLocation, scale.x, scale.y, scale.z, scale.w);
}

void Shader_CookTorrance::SetOffset(const glm::vec4 &offset)
{
	glUniform4f(m_offsetLocation, offset.x, offset.y, offset.z, offset.w);
}

void Shader_CookTorrance::SetCurrentPosition(const glm::vec4 &currentPos)
{
	glUniform4f(m_currentPosLocation, currentPos.x, currentPos.y, currentPos.z, currentPos.w);
}

void Shader_CookTorrance::SetHeightmap(unsigned int TextureUnit)
{
	glUniform1i(m_heightmapLocation, TextureUnit);
}

void Shader_CookTorrance::SetVtex(unsigned int TextureUnit)
{
	glUniform1i(m_vtexLocation, TextureUnit);
}

void Shader_CookTorrance::SetBtex(unsigned int TextureUnit)
{
	glUniform1i(m_bexLocation, TextureUnit);
}

void Shader_CookTorrance::SetBlendtex(int index, unsigned int TextureUnit)
{
	glUniform1i(m_bexBlendlocation[index], TextureUnit);
}

void Shader_CookTorrance::SetColortex(int index, unsigned int TextureUnit)
{
	glUniform1i(m_texLocation[index], TextureUnit);
}