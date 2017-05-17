#include "shaderspecular.h"
#include "shader.hpp"

bool Shader_Specular::Init()
{
	shader = LoadShaders("shader/specular_vert.glsl", "shader/specular_fragment.glsl");
	
	m_WVPLocation = glGetUniformLocation(shader, "gWVP");
	m_WorldMatrixLocation = glGetUniformLocation(shader, "gWorld");
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

//	m_normalLocation = glGetUniformLocation(shader, "texNormalmap");
	m_AOLocation = glGetUniformLocation(shader, "AOmap");

	m_ShowShadowLocation = glGetUniformLocation(shader, "gShowShadow");
    m_ShowAOLocation = glGetUniformLocation(shader, "gShowAO");
	m_ShowTextureLocation = glGetUniformLocation(shader, "gShowTexture");



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
	
	for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_shadowMapLocation); i++) {
		char Name[128] = { 0 };
		SNPRINTF(Name, sizeof(Name), "gShadowMap[%d]", i);
		m_shadowMapLocation[i] = glGetUniformLocation(shader,Name);

		if (m_shadowMapLocation[i] == 0xFFFFFFFF) {
			return false;
		}
	}

	for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_lightWVPLocation); i++) {
		char Name[128] = { 0 };
		SNPRINTF(Name, sizeof(Name), "gLightWVP[%d]", i);
		m_lightWVPLocation[i] = glGetUniformLocation(shader, Name);

		if (m_lightWVPLocation[i] == 0xFFFFFFFF) {
			return false;
		}
	}

	for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_cascadeEndClipSpace); i++) {
		char Name[128] = { 0 };
		SNPRINTF(Name, sizeof(Name), "gCascadeEndClipSpace[%d]", i);
		m_cascadeEndClipSpace[i] = glGetUniformLocation(shader,Name);

		if (m_cascadeEndClipSpace[i] == 0xFFFFFFFF) {
			return false;
		}
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

	if (m_dirLightLocation.AmbientIntensity == 0xFFFFFFFF ||
		m_WVPLocation == 0xFFFFFFFF ||
		m_WorldMatrixLocation == 0xFFFFFFFF ||
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
		m_bexLocation == 0xFFFFFFFF ||
//		m_normalLocation == 0xFFFFFFFF||
		m_AOLocation == 0xFFFFFFFF) {
		return false;
	}

	return true;
}

void Shader_Specular::SetWVP(const glm::mat4 &WVP)
{
	glUniformMatrix4fv(m_WVPLocation, 1, GL_FALSE, &WVP[0][0]);
}

void Shader_Specular::SetWorldMatrix(const glm::mat4 &WorldMatrix)
{
	glUniformMatrix4fv(m_WorldMatrixLocation, 1, GL_FALSE, &WorldMatrix[0][0]);
}

void Shader_Specular::SetDirectionalLight(const DirectionalLight& Light)
{
	glUniform3f(m_dirLightLocation.Color, Light.Color.x, Light.Color.y, Light.Color.z);
	glUniform1f(m_dirLightLocation.AmbientIntensity, Light.AmbientIntensity);
	glm::vec3 Direction = Light.Direction;
	Direction = glm::normalize(Direction);
	glUniform3f(m_dirLightLocation.Direction, Direction.x, Direction.y, Direction.z);
	glUniform1f(m_dirLightLocation.DiffuseIntensity, Light.DiffuseIntensity);
}

void Shader_Specular::SetEyeWorldPos(const glm::vec3& EyeWorldPos)
{
	glUniform3f(m_eyeWorldPosLocation, EyeWorldPos.x, EyeWorldPos.y, EyeWorldPos.z);
}

void Shader_Specular::SetMatSpecularIntensity(float Intensity)
{
	glUniform1f(m_matSpecularIntensityLocation, Intensity);
}

void Shader_Specular::SetMatSpecularPower(float Power)
{
	glUniform1f(m_matSpecularPowerLocation, Power);
}

void Shader_Specular::SetPointLights(unsigned int NumLights, const PointLight* pLights)
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

void Shader_Specular::SetSpotLights(unsigned int NumLights, const SpotLight* pLights)
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

void Shader_Specular::SetScale(const glm::vec4 &scale)
{
	glUniform4f(m_scaleLocation, scale.x, scale.y, scale.z, scale.w);
}

void Shader_Specular::SetOffset(const glm::vec4 &offset)
{
	glUniform4f(m_offsetLocation, offset.x, offset.y, offset.z, offset.w);
}

void Shader_Specular::SetCurrentPosition(const glm::vec4 &currentPos)
{
	glUniform4f(m_currentPosLocation, currentPos.x, currentPos.y, currentPos.z, currentPos.w);
}

void Shader_Specular::SetHeightmap(unsigned int TextureUnit)
{
	glUniform1i(m_heightmapLocation, TextureUnit);
}

void Shader_Specular::SetVtex(unsigned int TextureUnit)
{
	glUniform1i(m_vtexLocation, TextureUnit);
}

void Shader_Specular::SetBtex(unsigned int TextureUnit)
{
	glUniform1i(m_bexLocation, TextureUnit);
}

void Shader_Specular::SetBlendtex(int index, unsigned int TextureUnit)
{
	glUniform1i(m_bexBlendlocation[index], TextureUnit);
}

void Shader_Specular::SetColortex(int index, unsigned int TextureUnit)
{
	glUniform1i(m_texLocation[index], TextureUnit);
}

void Shader_Specular::SetNormaltex(unsigned int TextureUnit)
{
	glUniform1i(m_normalLocation, TextureUnit);
}

void Shader_Specular::SetAOTex(unsigned int TextureUnit)
{
	glUniform1i(m_AOLocation, TextureUnit);
}

void Shader_Specular::SetShowAO(int flag)
{
	glUniform1i(m_ShowAOLocation, flag);
}

void Shader_Specular::SetLightWVP(int CascadeIndex, const glm::mat4 & LightWVP)
{
	glUniformMatrix4fv(m_lightWVPLocation[CascadeIndex], 1, GL_TRUE, &LightWVP[0][0]);
}

void Shader_Specular::SetCascadeEndClipSpace(int CascadeIndex, float End)
{
	glUniform1f(m_cascadeEndClipSpace[CascadeIndex], End);
}

void Shader_Specular::SetShadowMapTextureUnit(unsigned int TextureUnit)
{
	glUniform1i(m_shadowMapLocation[0], TextureUnit);
	glUniform1i(m_shadowMapLocation[1], TextureUnit + 1);
	glUniform1i(m_shadowMapLocation[2], TextureUnit + 2);
}

void Shader_Specular::SetShowShadow(int flag)
{
	glUniform1i(m_ShowShadowLocation, flag);
}

void Shader_Specular::SetShowTexture(int flag)
{
	glUniform1i(m_ShowTextureLocation, flag);
}