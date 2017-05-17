#ifndef SHADER_SPECULAR
#define SHADER_SPECULAR

#include "shaderuniform.h"
#include "light.h"

#define NUM_CASCADES 3

class Shader_Specular : public ShaderUniform {
public:
	static const unsigned int MAX_POINT_LIGHTS = 2;
	static const unsigned int MAX_SPOT_LIGHTS = 2;

	virtual bool Init();

	void SetWVP(const glm::mat4 &WVP);

	void SetWorldMatrix(const glm::mat4 &WorldMatrix);

	void SetDirectionalLight(const DirectionalLight& Light);

	void SetEyeWorldPos(const glm::vec3& EyeWorldPos);

	void SetMatSpecularIntensity(float Intensity);

	void SetMatSpecularPower(float Power);

	void SetPointLights(unsigned int NumLights, const PointLight* pLights);

	void SetSpotLights(unsigned int NumLights, const SpotLight* pLights);

	void SetScale(const glm::vec4 &scale);

	void SetOffset(const glm::vec4 &offset);

	void SetCurrentPosition(const glm::vec4 &currentPos);

	void SetHeightmap(unsigned int TextureUnit);

	void SetVtex(unsigned int TextureUnit);

	void SetBtex(unsigned int TextureUnit);

	void SetBlendtex(int index, unsigned int TextureUnit);

	void SetColortex(int index, unsigned int TextureUnit);

	void SetNormaltex(unsigned int TextureUnit);

	void SetAOTex(unsigned int TextureUnit);

	void SetShowAO(int flag);

	void SetShowShadow(int flag);

	void SetShowTexture(int flag);

	void SetLightWVP(int CascadeIndex, const glm::mat4& LightWVP);

	void SetCascadeEndClipSpace(int CascadeIndex, float End);

	void SetShadowMapTextureUnit(unsigned int TextureUnit);
private:
	GLuint m_WVPLocation;
	GLuint m_WorldMatrixLocation;
	GLuint m_eyeWorldPosLocation;
	GLuint m_matSpecularIntensityLocation;
	GLuint m_matSpecularPowerLocation;
	GLuint m_numPointLightsLocation;
	GLuint m_numSpotLightsLocation;

	GLuint m_vtexLocation;
	GLuint m_heightmapLocation;
	GLuint m_scaleLocation;
	GLuint m_offsetLocation;
	GLuint m_currentPosLocation;

	GLuint m_texLocation[6];
	GLuint m_bexLocation;
	GLuint m_bexBlendlocation[2];

	GLuint m_normalLocation;
	GLuint m_AOLocation;

	GLuint m_ShowAOLocation;
	GLuint m_ShowShadowLocation;
	GLuint m_ShowTextureLocation;

	GLuint m_lightWVPLocation[NUM_CASCADES];
	GLuint m_cascadeEndClipSpace[NUM_CASCADES];
	GLuint m_shadowMapLocation[NUM_CASCADES];

	struct {
		GLuint Color;
		GLuint AmbientIntensity;
		GLuint DiffuseIntensity;
		GLuint Direction;
	} m_dirLightLocation;

	struct {
		GLuint Color;
		GLuint AmbientIntensity;
		GLuint DiffuseIntensity;
		GLuint Position;
		struct {
			GLuint Constant;
			GLuint Linear;
			GLuint Exp;
		} Atten;
	} m_pointLightsLocation[MAX_POINT_LIGHTS];

	struct {
		GLuint Color;
		GLuint AmbientIntensity;
		GLuint DiffuseIntensity;
		GLuint Position;
		GLuint Direction;
		GLuint Cutoff;
		struct {
			GLuint Constant;
			GLuint Linear;
			GLuint Exp;
		} Atten;
	} m_spotLightsLocation[MAX_SPOT_LIGHTS];
};


#endif
