#ifndef TERRAIN_H
#define TERRAIN_H

#include <GL/glew.h>
#include <vector>
#include <string>
#include "shading.h"
#include "shaderuniform.h"
#include "shaderspecular.h"
#include "shadercooktorrance.h"
#include "shaderphong.h"
#include "shadernull.h"
#include "VTex.h"
#include "VTshader.h"
#include "framebuffer.h"
#include "sat.h"
#include "shadershadow.h"
#include "shadowmap.h"


#define NUM_FRUSTUM_CORNERS 8

struct OrthoProjInfo
{
	float r;        // right
	float l;        // left
	float b;        // bottom
	float t;        // top
	float n;        // z near
	float f;        // z far
};

class Block {
public:
	Block(int index)
	{
		blockID = index;

		visible = true;

		isparent = false;
	}

	~Block()
	{

	}

	int blockID;
	bool visible;
	bool isparent;
	ShadingLOD block_lod;
	glm::vec2 offset;
	float scale;
};

const int BASICTEXNUM = 6;
const std::string texnames[] = { "xsc_db001_1748.tga", "fb_jjc_cd002_2161.tga",
"fb_jjc_cd001_2161.tga", "xsc_db002_1748.tga",
"xsc_dbcd007_1201.tga", "xsc_dbsk006_1201.tga",
"xsc_db001_1748_n.tga", "xsc_dbcd007_1201_n.tga",
"xsc_dbsk006_1201_n.tga" };

class Terrain {
public:
	Terrain()
	{
		for (int i = 0; i < 4; i++)
		{
			isshaderinit[i] = false;
		}
		m_scale = 0;
		m_Shadowflag = 1;
		AOflag = 0;
		m_Textureflag = 1;

		m_directionalLight.Color = glm::vec3(1.0f, 1.0f, 1.0f);
		m_directionalLight.AmbientIntensity = 0.0f;
		m_directionalLight.DiffuseIntensity = 0.0f;
		m_directionalLight.Direction = glm::vec3(1.0f, -1.0, 0.0);
	}

	~Terrain(){
		m_Block.clear();
		vertices.clear();
	}

	bool Init();

	bool InitTexture();

	bool InitShader();

	void VertexInit();

	bool frustumCull(float scale, glm::vec2 offset);

	void updateHtex(float currentX, float currentZ);

	void updateBtex();

	void updateBlock(float currentX, float currentZ,Block& tblock);

	void render();

	void CalcOrthoProjs();

	std::vector<Block> m_Block;
	std::vector<Block> m_child;

	GLuint texID[BASICTEXNUM];

	VTex htex; // height map
	VTex btex; // blend map

	glm::mat4 m_WVP;
	glm::mat4 m_World;
	glm::mat4 m_View;
	float m_ar;
	float m_fov;
	
	DirectionalLight m_directionalLight;
	float m_scale;
    glm::vec3 m_camerapos;
	glm::vec3 currentPos;

	int AOflag;
	int m_Textureflag;
	int m_Shadowflag;

//grid »æÖÆ
	std::vector<GLfloat> vertices;
	GLuint vao;
	GLuint vbo;

	VTshaderuniform m_vtshader;
	FrameBuffer feedback, m_texture;

	SAT sat;

//LODShader
	Shader_Specular m_specular;
	Shader_CookTorrance m_cooktorrance;
	Shader_Phong m_phong;
	Shader_NULL m_null;
	bool isshaderinit[4];

//Cascaded Shadow Map
	void ShadowMapPass();
	void InitShadow();

	ShaderShadow m_shadow;
	CascadedShadowMapFBO m_csmFBO;
	float m_cascadeEnd[NUM_CASCADES + 1];
	OrthoProjInfo m_shadowOrthoProjInfo[NUM_CASCADES];
};

#endif