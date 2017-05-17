#include "terrain.h"
#include <ImageMagick/Magick++.h>
#include <iostream>
#include <SOIL/SOIL.h>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

void Terrain::VertexInit()
{
	for (int j = 0; j < GRID + 1; j++)
		for (int i = 0; i < GRID + 2; i++) {

			for (int k = 0; k < ((i == 0) ? 2 : 1); k++) {
				//			for (int k = 0; k < 1; k++) {
				vertices.push_back((float)i / GRID);
				vertices.push_back(0);
				vertices.push_back((float)j / GRID);
				//                printf("%d, %d, %d\n", i, 0, j);
			}

			j++;
			for (int k = 0; k < ((i == GRID + 1) ? 2 : 1); k++) {
				//		for (int k = 0; k < 1; k++) {
				vertices.push_back((float)i / GRID);
				vertices.push_back(0);
				vertices.push_back((float)j / GRID);
				//                printf("%d, %d, %d\n", i, 0, j);
			}
			j--;
		}
	for (int i = 0; i < 4; i++) {
		int tmp;
		tmp = (i < 2) ? 1 : 0;
		for (int j = 0; j < 2; j++) {
			vertices.push_back((float)(GRID + tmp) / GRID);
			vertices.push_back(0);
			vertices.push_back((float)GRID / GRID);
		}
	}

	//T-junction removal
	float x, y;
	x = y = GRID;
	for (int i = 0; i < GRID * 2; x -= 0.5, i++) {
		vertices.push_back((float)x / GRID);
		vertices.push_back(0);
		vertices.push_back((float)y / GRID);
	}
	vertices.push_back((float)x / GRID);
	vertices.push_back(0);
	vertices.push_back((float)y / GRID);
	for (int i = 0; i < GRID * 2; y -= 0.5, i++) {
		vertices.push_back((float)x / GRID);
		vertices.push_back(0);
		vertices.push_back((float)y / GRID);
	}
	vertices.push_back((float)x / GRID);
	vertices.push_back(0);
	vertices.push_back((float)y / GRID);
	for (int i = 0; i < GRID * 2; x += 0.5, i++) {
		vertices.push_back((float)x / GRID);
		vertices.push_back(0);
		vertices.push_back((float)y / GRID);
	}
	vertices.push_back((float)x / GRID);
	vertices.push_back(0);
	vertices.push_back((float)y / GRID);
	for (int i = 0; i < GRID * 2; y += 0.5, i++) {
		vertices.push_back((float)x / GRID);
		vertices.push_back(0);
		vertices.push_back((float)y / GRID);
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat),
		&vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
		(GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

bool Terrain::InitTexture()
{
/*
	string filename;
	for (int i = 0; i < BASICTEXNUM; i++) {
		glGenTextures(1, &texID[i]);
		filename = "textures/" + texnames[i];
		Magick::Image m_pImage;
		Magick::Blob m_blob;
		try {
			m_pImage.read(filename);
			m_pImage.write(&m_blob, "RGBA");
		}
		catch (Magick::Error& Error) {
			std::cout << "Error loading texture '" << filename.c_str() << "': " << Error.what() << std::endl;
			return false;
		}
		glBindTexture(GL_TEXTURE_2D, texID[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
			GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4.0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_pImage.columns(), m_pImage.rows(), 0, GL_RGBA,
			GL_UNSIGNED_BYTE, m_blob.data());
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
*/

	string filename;
	int w, h, channels, type;
	for (int i = 0; i < BASICTEXNUM; i++) {
		glGenTextures(1, &texID[i]);
		filename = "textures/" + texnames[i];
		unsigned char* img = SOIL_load_image(filename.c_str(),
			&w, &h, &channels, SOIL_LOAD_AUTO);
		glBindTexture(GL_TEXTURE_2D, texID[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
			GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4.0);
		type = (channels == 4) ? GL_RGBA : GL_RGB;
		glTexImage2D(GL_TEXTURE_2D, 0, type, w, h, 0, type,
			GL_UNSIGNED_BYTE, img);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(img);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	return true;
}

bool Terrain::InitShader()
{
	m_cooktorrance.block_index = new bool[m_Block.size()];
	m_phong.block_index = new bool[m_Block.size()];
	m_specular.block_index = new bool[m_Block.size()];
	m_null.block_index = new bool[m_Block.size()];

	memset(m_cooktorrance.block_index, false, m_Block.size());
	memset(m_phong.block_index, false, m_Block.size());
	memset(m_specular.block_index, false, m_Block.size());
	memset(m_null.block_index, false, m_Block.size());

	for (int i = 0; i < m_Block.size(); i++)
	{
		int shaderlod = m_Block[i].block_lod.BRDF;
			switch (shaderlod)
			{
			case 0:
				m_cooktorrance.block_index[i] = true;
				if (!isshaderinit[shaderlod])
				m_cooktorrance.Init();
				break;
			case 1:
				m_phong.block_index[i] = true;
				if (!isshaderinit[shaderlod])
				m_phong.Init();
				break;
			case 2:
				m_specular.block_index[i] = true;
				if (!isshaderinit[shaderlod])
				{
					isshaderinit[shaderlod] = true;
					m_specular.Init();
					m_specular.Enable();
					for (int i = 0; i < BASICTEXNUM; i++)
					{
						m_specular.SetColortex(i, 5 + i);
					}
					for (int i = 0; i < 2; i++)
						m_specular.SetBlendtex(i, 3 + i);
					m_specular.SetAOTex(11);
					m_specular.SetVtex(0);
					m_specular.SetHeightmap(1);
					m_specular.SetBtex(2);
				// shadow
					m_specular.SetShadowMapTextureUnit(12);
					for (int i = 0; i < NUM_CASCADES ; i++) {
					//glm::mat4 projection = PerspectiveProjection(glm::radians(fov), (float)WIDTH / (float)HEIGHT,znear, zfar);
					glm::mat4 projection = glm::perspective(glm::radians(fov), (float)WIDTH / (float)HEIGHT, znear, zfar);
					glm::vec4 vView(0.0f, 0.0f, -m_cascadeEnd[i + 1], 1.0f);
					glm::vec4 vClip = projection * vView;
					m_specular.SetCascadeEndClipSpace(i, vClip.z);
				}
					glUseProgram(0);
				}
				break;
			case 3:
				m_null.block_index[i] = true;
				if (!isshaderinit[shaderlod])
				{
					isshaderinit[shaderlod] = true;
					m_null.Init();
					m_null.Enable();
					for (int i = 0; i < BASICTEXNUM; i++)
					{
						m_null.SetColortex(i, 5 + i);
					}
					for (int i = 0; i < 2; i++)
						m_null.SetBlendtex(i, 3 + i);
					m_null.SetVtex(0);
					m_null.SetHeightmap(1);
					m_null.SetBtex(2);
					glUseProgram(0);
				}
				break;
			}
    }
	return true;
}

void Terrain::InitShadow()
{
	if (!m_csmFBO.Init(WIDTH, HEIGHT)) {
		return ;
	}

	m_cascadeEnd[0] = znear;
	m_cascadeEnd[1] = 1000.0f,
	m_cascadeEnd[2] = 1500.0f,
	m_cascadeEnd[3] = zfar;

	m_shadow.Init();
	m_shadow.Enable();
	m_shadow.SetVtex(0);
	m_shadow.SetHeightmap(1);
	glUseProgram(0);
}

bool Terrain::Init()
{
//初始化点
   VertexInit();
//初始化block
   int index = 0;
   float r = MAXSCALE;
   for (int l = 0; l < LEVEL; l++) {
	   for (int i = -2; i < 2; i++)
		   for (int j = -2; j < 2; j++) {
			   if (l != LEVEL - 1 && i != -2 && i != 1 && j != -2 && j != 1)
				   continue;
			   Block new_block(index++);
			   new_block.scale = r;
			   new_block.offset = glm::vec2(r*i, r*j);
			   new_block.block_lod.BRDF = 2;
			   m_Block.push_back(new_block);
		   }
	   r *= 0.5;
   }

   InitShadow();

   InitShader();
//初始化，合成texture
	if (!InitTexture()) return false;

//初始化virtual texture
	m_vtshader.Init();
	m_vtshader.Enable();
	m_vtshader.SetVtex(0);
	m_vtshader.SetHeightmap(1);
	glUseProgram(0);
	//初始化heightmap和blend texture
	htex.init(CHUNKSIZE, CHUNKNUMBER, true, 1);
	btex.init(BLENDSIZE, CHUNKNUMBER, false, LEVELOFBLENDTEX - 3);
	
	m_texture.init(WIDTH, HEIGHT);
	feedback.init(FEEDBACK_WIDTH, FEEDBACK_HEIGHT);
	
	sat.loadMaxAndMin();

	m_directionalLight.AmbientIntensity = 0.7f;
	m_directionalLight.DiffuseIntensity = 0.5f;
	m_directionalLight.Color = glm::vec3(1.0f, 1.0f, 1.0f);
	m_directionalLight.Direction = glm::vec3(-1.0f, -1.0f, 0.0f);
	
	

	return true;
}

bool Terrain::frustumCull(float scale, glm::vec2 offset)
{
	int xs, xb, ys, yb, zs, zb;
	xs = xb = ys = yb = zs = zb = 0;
	for (int i = 0; i <= 1; i++)
		for (int j = 0; j <= 1; j++)
			for (int k = -1; k <= 1; k += 2) {
				glm::vec4 pos = glm::vec4(glm::vec3(scale,1,scale) * glm::vec3(i, k * 1000, j) + glm::vec3(offset.x, 0, offset.y), 1.0);
				pos = m_WVP * pos;
				xs = (pos.x < -pos.w) ? xs + 1 : xs;
				xb = (pos.x > pos.w) ? xb + 1 : xb;
				ys = (pos.y < -pos.w) ? ys + 1 : ys;
				yb = (pos.y > pos.w) ? yb + 1 : yb;
				zs = (pos.z < -pos.w) ? zs + 1 : zs;
				zb = (pos.z > pos.w) ? zb + 1 : zb;
			}
	return xs == 8 || xb == 8 || ys == 8 || yb == 8 || zs == 8 || zb == 8;
}

void Terrain::updateHtex(float currentX, float currentZ)
{
	sat.loadSATData(currentX, currentZ);

	htex.clear();

	m_child.clear();
	m_specular.childindex.clear();
	m_null.childindex.clear();
	m_cooktorrance.childindex.clear();
	m_phong.childindex.clear();

	for (int i = 0; i < m_Block.size(); i++)
	{
		m_Block[i].isparent = false;
		updateBlock(currentX, currentZ,m_Block[i]);
	}

	for (int i = 0; i < m_child.size(); i++)
	{
		updateBlock(currentX, currentZ, m_child[i]);
	}
	htex.update(1, MAXSCALE, currentX, currentZ);
	htex.generateTex();
}

void Terrain::updateBlock(float currentX, float currentZ, Block& tblock)
{
	int xs, xb, ys, yb, zs, zb;
	xs = xb = ys = yb = zs = zb = 0;
	float minx, miny, minz, maxx, maxy, maxz;
	minx = miny = minz = 1;
	maxx = maxy = maxz = -1;
	for (int i = 0; i <= 1; i++)
		for (int j = 0; j <= 1; j++)
			for (int k = -1; k <= 1; k += 2) {
				glm::vec4 pos = glm::vec4(glm::vec3(tblock.scale, 1, tblock.scale) * glm::vec3(i, k * 200, j) + glm::vec3(tblock.offset.x, 0, tblock.offset.y), 1.0);
				pos = m_WVP * pos;
				xs = (pos.x < -pos.w) ? xs + 1 : xs;
				xb = (pos.x > pos.w) ? xb + 1 : xb;
				ys = (pos.y < -pos.w) ? ys + 1 : ys;
				yb = (pos.y > pos.w) ? yb + 1 : yb;
				zs = (pos.z < -pos.w) ? zs + 1 : zs;
				zb = (pos.z > pos.w) ? zb + 1 : zb;
				minx = clampf(Min(pos.x / pos.w, minx), -1, 1);
				miny = clampf(Min(pos.y / pos.w, miny), -1, 1);
				minz = clampf(Min(pos.z / pos.w, minz), -1, 1);
				maxx = clampf(Max(pos.x / pos.w, maxx), -1, 1);
				maxy = clampf(Max(pos.y / pos.w, maxy), -1, 1);
				maxz = clampf(Max(pos.z / pos.w, maxz), -1, 1);
			}
	if (xs == 8 || xb == 8 || ys == 8 || yb == 8 || zs == 8 || zb == 8)
	{
		tblock.visible = false;
		return;
	}

	float ratio = VIEWCHUNKNUMBER / float(CHUNKNUMBER);
	float lx = currentX + tblock.offset.x / MAXSCALE / 4.0 * ratio;
	float rx = currentX + (tblock.offset.x + tblock.scale) / MAXSCALE / 4.0 * ratio;
	float ly = currentZ + tblock.offset.y / MAXSCALE / 4.0  * ratio;
	float ry = currentZ + (tblock.offset.y + tblock.scale) / MAXSCALE / 4.0  * ratio;

	double ma = sat.getMaxDiff(
		lx * CHUNKNUMBER * CHUNKSIZE,
		ly * CHUNKNUMBER * CHUNKSIZE,
		rx * CHUNKNUMBER * CHUNKSIZE,
		ry * CHUNKNUMBER * CHUNKSIZE);

	float area = (maxx-minx)*(maxy-miny);
	ma /= GRID;
//	glm::vec2 center = glm::vec2((lx + rx) / 2 * CHUNKNUMBER * CHUNKSIZE, (ly + ry) / 2 * CHUNKNUMBER * CHUNKSIZE);
//	double distance = sqrt(pow(center.x, 2.0) + pow(center.y, 2.0));
	double distance = 2.0 + (1.0 + minz) / 2; //view (0.0-1.0)
	double sError =   ma * area / tan(fov/2) / distance;

	if (sError > 0.15)
	{
		tblock.isparent = true;
		Block tempchild1(m_Block.size() + m_child.size());
		tempchild1.scale = tblock.scale / 2;
		tempchild1.offset = tblock.offset;
		tempchild1.block_lod.BRDF = 2;
		m_specular.childindex.push_back(m_child.size());
		m_child.push_back(tempchild1);

		Block tempchild2(m_Block.size() + m_child.size());
		tempchild2.scale = tblock.scale / 2;
		tempchild2.offset = tblock.offset + glm::vec2(tblock.scale / 2,0.0);
		tempchild2.block_lod.BRDF = 2;
		m_specular.childindex.push_back(m_child.size());
		m_child.push_back(tempchild2);

		Block tempchild3(m_Block.size() + m_child.size());
		tempchild3.scale = tblock.scale / 2;
		tempchild3.offset = tblock.offset + glm::vec2(0.0, tblock.scale / 2);
		tempchild3.block_lod.BRDF = 2;
		m_specular.childindex.push_back(m_child.size());
		m_child.push_back(tempchild3);

		Block tempchild4(m_Block.size() + m_child.size());
		tempchild4.scale = tblock.scale / 2;
		tempchild4.offset = tblock.offset + glm::vec2(tblock.scale / 2, tblock.scale / 2);
		tempchild4.block_lod.BRDF = 2;
		m_specular.childindex.push_back(m_child.size());
		m_child.push_back(tempchild4);
	}
}

void Terrain::updateBtex()
{
	static float data[FEEDBACK_WIDTH * FEEDBACK_HEIGHT * 4];
	feedback.begin();
	glReadPixels(0, 0, FEEDBACK_WIDTH, FEEDBACK_HEIGHT, GL_RGBA, GL_FLOAT, (void*)&data);
	feedback.end();
	btex.clear();
	for (int i = 0; i < FEEDBACK_HEIGHT; i++)
		for (int j = 0; j < FEEDBACK_WIDTH; j++) {
			float *p = data + (i * FEEDBACK_WIDTH + j) * 4;
			if (fabs(p[3]) < 1e-6)
				continue;
			p[2] += log2((float)FEEDBACK_WIDTH / (float)WIDTH);
			int l = (int)round(p[2] - 0.5);
			int r = (int)round(p[2] + 0.5);
			int x, y;
			x = floor(p[0] / (1.0 / CHUNKNUMBER) - 1e-6);
			y = floor(p[1] / (1.0 / CHUNKNUMBER) - 1e-6);
			clamp(x, 0, CHUNKNUMBER - 1);
			clamp(y, 0, CHUNKNUMBER - 1);
			clamp(l, 0, btex.getMaxLevel() - 1);
			clamp(r, 0, btex.getMaxLevel() - 1);
			btex.update(l, x >> l, y >> l);
			if (l != r)
				btex.update(r, x >> r, y >> r);
		}
	btex.generateTex();
}

void Terrain::render()
{
//vt render
	feedback.clear();
	feedback.begin();
	glViewport(0, 0, FEEDBACK_WIDTH, FEEDBACK_HEIGHT);

	m_vtshader.Enable();
	glBindVertexArray(vao);
	updateHtex(currentPos.x / CHUNKNUMBER / CHUNKSIZE, currentPos.z / CHUNKNUMBER / CHUNKSIZE);
	m_vtshader.SetCurrentPosition(glm::vec4(currentPos.x / CHUNKNUMBER / CHUNKSIZE, currentPos.z / CHUNKNUMBER / CHUNKSIZE, currentPos.y, 0.0));
	m_vtshader.SetWVP(m_WVP);

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, htex.getTex());

	vector<PTex*> ptexes = htex.getPtexes();
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, ptexes[0]->getTex());

	for (int i = 0; i < m_Block.size(); i++)
	{
		if (m_Block[i].isparent || !m_Block[i].visible) continue;
		m_vtshader.SetScale(glm::vec4(m_Block[i].scale, 1, m_Block[i].scale, 1));
		m_vtshader.SetOffset(glm::vec4(m_Block[i].offset.x, 0, m_Block[i].offset.y, 0));
		glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size() / 3);
	}

	for (int i = 0; i < m_child.size(); i++)
	{
		if (m_child[i].visible && !m_child[i].isparent)
		{
			m_vtshader.SetScale(glm::vec4(m_child[i].scale, 1, m_child[i].scale, 1));
			m_vtshader.SetOffset(glm::vec4(m_child[i].offset.x, 0, m_child[i].offset.y, 0));
			glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size() / 3);
		}
	}

	glBindVertexArray(0);
	glUseProgram(0);
	feedback.end();

    ShadowMapPass();

	// Clear the screen
	glClearDepth(1.0f);
	glClearColor(0.46f, 0.46f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
//render
	updateBtex();
	//m_texture.clear();
	//m_texture.begin();
	glViewport(0, 0, WIDTH, HEIGHT);
//light
	m_scale += 0.0057f;
	PointLight pl[2];
	pl[0].DiffuseIntensity = 0.5f;
	pl[0].Color = glm::vec3(1.0f, 0.5f, 0.0f);
	pl[0].Position = glm::vec3(580, 59, 441 + 20 * (cosf(m_scale) + 1.0f) / 2.0f);
	pl[0].Position = pl[0].Position - glm::vec3(currentPos.x, 0, currentPos.z);
	pl[0].Position.x = pl[0].Position.x / GRID;
	pl[0].Position.z = pl[0].Position.z / GRID;
	pl[0].Attenuation.Linear = 0.1f;
	pl[1].DiffuseIntensity = 0.5f;
	pl[1].Color = glm::vec3(0.0f, 0.5f, 1.0f);
	pl[1].Position = glm::vec3(590, 59, 441 + 20 * (sinf(m_scale) + 1.0f) / 2.0f);
	pl[1].Position = pl[1].Position - glm::vec3(currentPos.x, 0, currentPos.z);
	pl[1].Position.x = pl[1].Position.x / GRID;
	pl[1].Position.z = pl[1].Position.z / GRID;
	pl[1].Attenuation.Linear = 0.1f;


//cook torrance 
	if (isshaderinit[0])
	{

	}

//phong
	if (isshaderinit[1])
	{

	}

//specular
	if (isshaderinit[2])
	{
		m_specular.Enable();
//shadow
		m_specular.SetShowShadow(m_Shadowflag);
		m_specular.SetShowTexture(m_Textureflag);

		m_csmFBO.BindForReading();

		//glm::mat4 lightview = ViewProjection(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0) + m_directionalLight.Direction, glm::vec3(0, 1, 0));
		glm::mat4 lightview = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0) + m_directionalLight.Direction, glm::vec3(0, 1, 0));
		for (int i = 0; i < NUM_CASCADES; i++) {
			//glm::mat4 orthomatrix = OrthoProjection(m_shadowOrthoProjInfo[i].l, m_shadowOrthoProjInfo[i].r, m_shadowOrthoProjInfo[i].b, m_shadowOrthoProjInfo[i].t, m_shadowOrthoProjInfo[i].n, m_shadowOrthoProjInfo[i].f);
			glm::mat4 orthomatrix = glm::ortho(m_shadowOrthoProjInfo[i].l, m_shadowOrthoProjInfo[i].r, m_shadowOrthoProjInfo[i].b, m_shadowOrthoProjInfo[i].t, m_shadowOrthoProjInfo[i].n, m_shadowOrthoProjInfo[i].f);
			glm::mat4 lightvp = orthomatrix * lightview * m_World;
			m_specular.SetLightWVP(i, lightvp);
		}

		m_specular.SetShowAO(AOflag);
		glBindVertexArray(vao);
		m_specular.SetCurrentPosition(glm::vec4(currentPos.x / CHUNKNUMBER / CHUNKSIZE, currentPos.z / CHUNKNUMBER / CHUNKSIZE, currentPos.y, 0.0));
	
		m_specular.SetWVP(m_WVP);
		m_specular.SetWorldMatrix(m_World);
		//m_specular.SetPointLights(2, pl);
		m_specular.SetDirectionalLight(m_directionalLight);
		m_specular.SetEyeWorldPos(m_camerapos);
		m_specular.SetMatSpecularIntensity(1.0f);
		m_specular.SetMatSpecularPower(1.0f);

		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, htex.getTex());

		vector<PTex*> ptexes = htex.getPtexes();
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, ptexes[0]->getTex());

		glActiveTexture(GL_TEXTURE11);
		glBindTexture(GL_TEXTURE_2D, ptexes[1]->getTex());

		vector<PTex*> bptexes = btex.getPtexes();
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, btex.getTex());

		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, bptexes[0]->getTex());

		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, bptexes[1]->getTex());

		for (int i = 0; i < BASICTEXNUM; i++) {
			glActiveTexture(GL_TEXTURE5 + i);
			glBindTexture(GL_TEXTURE_2D, texID[i]);
		}
		
		for (int i = 0; i < m_Block.size(); i++)
		{
			if(m_specular.block_index[i] && !m_Block[i].isparent && m_Block[i].visible)
			{
				m_specular.SetScale(glm::vec4(m_Block[i].scale, 1, m_Block[i].scale, 1));
				m_specular.SetOffset(glm::vec4(m_Block[i].offset.x, 0, m_Block[i].offset.y, 0));
				glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size() / 3);
			}
		}

	for (int i = 0; i < m_specular.childindex.size(); i++)
		{
			int childindex = m_specular.childindex[i];
			if (m_child[childindex].visible && !m_child[childindex].isparent)
			{
				m_specular.SetScale(glm::vec4(m_child[childindex].scale, 1, m_child[childindex].scale, 1));
				m_specular.SetOffset(glm::vec4(m_child[childindex].offset.x, 0, m_child[childindex].offset.y, 0));
				glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size() / 3);
			}		
		}
		glBindVertexArray(0);
		glUseProgram(0);
	}
//null
	if (isshaderinit[3])
	{
		m_null.Enable();
		glBindVertexArray(vao);
		m_null.SetCurrentPosition(glm::vec4(currentPos.x / CHUNKNUMBER / CHUNKSIZE, currentPos.z / CHUNKNUMBER / CHUNKSIZE, currentPos.y, 0.0));

		m_null.SetWVP(m_WVP);

		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, htex.getTex());
		vector<PTex*> ptexes = htex.getPtexes();
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, ptexes[0]->getTex());
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, btex.getTex());
		vector<PTex*> bptexes = btex.getPtexes();
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, bptexes[0]->getTex());
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, bptexes[1]->getTex());


		for (int i = 0; i < BASICTEXNUM; i++) {
			glActiveTexture(GL_TEXTURE5 + i);
			glBindTexture(GL_TEXTURE_2D, texID[i]);
		}
		
		for (int i = 0; i < m_Block.size(); i++)
		{
			if (m_null.block_index[i] && !m_Block[i].isparent && m_Block[i].visible)
			{
				m_null.SetScale(glm::vec4(m_Block[i].scale, 1, m_Block[i].scale, 1));
				m_null.SetOffset(glm::vec4(m_Block[i].offset.x, 0, m_Block[i].offset.y, 0));
				glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size() / 3);
			}
		}
		for (int i = 0; i < m_specular.childindex.size(); i++)
		{
			int childindex = m_specular.childindex[i];
			if (m_child[childindex].visible && !m_child[childindex].isparent)
			{
				m_specular.SetScale(glm::vec4(m_child[i].scale, 1, m_child[i].scale, 1));
				m_specular.SetOffset(glm::vec4(m_child[i].offset.x, 0, m_child[i].offset.y, 0));
				glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size() / 3);
			}
		}
		glBindVertexArray(0);
		glUseProgram(0);
	}
	//m_texture.end();
	//m_texture.savetoImage();

	//glViewport(0, 0, WIDTH / 5, HEIGHT / 5);
	//m_texture.render();
}

void Terrain::CalcOrthoProjs()
{
	glm::mat4 CamInv = glm::inverse(m_View);

	//glm::mat4 lightview = ViewProjection(glm::vec3(0,0,0), glm::vec3(0, 0, 0) + m_directionalLight.Direction, glm::vec3(0,1,0));
	glm::mat4 lightview = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0) + m_directionalLight.Direction, glm::vec3(0, 1, 0));

	float tanHalfHFOV = tanf(ToRadian(m_fov / 2.0f));
	float tanHalfVFOV = tanf(ToRadian((m_fov * m_ar) / 2.0));

	for (int i = 0; i < NUM_CASCADES; i++) {
		float xn = m_cascadeEnd[i] * tanHalfHFOV;
		float xf = m_cascadeEnd[i + 1] * tanHalfHFOV;
		float yn = m_cascadeEnd[i] * tanHalfVFOV;
		float yf = m_cascadeEnd[i + 1] * tanHalfVFOV;

		glm::vec4 frustumCorners[NUM_FRUSTUM_CORNERS] = {
			// near face
			glm::vec4(xn,   yn, -m_cascadeEnd[i], 1.0),
			glm::vec4(-xn,  yn, -m_cascadeEnd[i], 1.0),
			glm::vec4(xn,  -yn, -m_cascadeEnd[i], 1.0),
			glm::vec4(-xn, -yn, -m_cascadeEnd[i], 1.0),

			// far face
			glm::vec4(xf,   yf, -m_cascadeEnd[i + 1], 1.0),
			glm::vec4(-xf,  yf, -m_cascadeEnd[i + 1], 1.0),
			glm::vec4(xf,  -yf, -m_cascadeEnd[i + 1], 1.0),
			glm::vec4(-xf, -yf, -m_cascadeEnd[i + 1], 1.0)
		};

		glm::vec4 frustumCornersL[NUM_FRUSTUM_CORNERS];

		float minX = std::numeric_limits<float>::max();
		float maxX = std::numeric_limits<float>::min();
		float minY = std::numeric_limits<float>::max();
		float maxY = std::numeric_limits<float>::min();
		float minZ = std::numeric_limits<float>::max();
		float maxZ = std::numeric_limits<float>::min();
		for (int j = 0; j < NUM_FRUSTUM_CORNERS; j++) {
			glm::vec4 vW = CamInv * frustumCorners[j];
			frustumCornersL[j] = lightview * vW;
			
			minX = min(minX, frustumCornersL[j].x);
			maxX = max(maxX, frustumCornersL[j].x);
			minY = min(minY, frustumCornersL[j].y);
			maxY = max(maxY, frustumCornersL[j].y);
			minZ = min(minZ, frustumCornersL[j].z);
			maxZ = max(maxZ, frustumCornersL[j].z);
		}

		m_shadowOrthoProjInfo[i].r = maxX;
		m_shadowOrthoProjInfo[i].l = minX;
		m_shadowOrthoProjInfo[i].b = minY;
		m_shadowOrthoProjInfo[i].t = maxY;
		m_shadowOrthoProjInfo[i].f = maxZ;
		m_shadowOrthoProjInfo[i].n = minZ;
	}
}

void Terrain::ShadowMapPass()
{
	glClear(GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, WIDTH, HEIGHT);

	CalcOrthoProjs();

	m_shadow.Enable();

	//glm::mat4 lightview = ViewProjection(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0) + m_directionalLight.Direction, glm::vec3(0, 1, 0));
	glm::mat4 lightview = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0) + m_directionalLight.Direction, glm::vec3(0, 1, 0));
	
	glBindVertexArray(vao);

	m_shadow.SetCurrentPosition(glm::vec4(currentPos.x / CHUNKNUMBER / CHUNKSIZE, currentPos.z / CHUNKNUMBER / CHUNKSIZE, currentPos.y, 0.0));

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, htex.getTex());

	vector<PTex*> ptexes = htex.getPtexes();
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, ptexes[0]->getTex());

	for (int i = 0; i < NUM_CASCADES; i++) {
		m_csmFBO.BindForWriting(i);
		glClear(GL_DEPTH_BUFFER_BIT);
		//glm::mat4 orthomatrix = OrthoProjection(m_shadowOrthoProjInfo[i].l, m_shadowOrthoProjInfo[i].r, m_shadowOrthoProjInfo[i].b, m_shadowOrthoProjInfo[i].t, m_shadowOrthoProjInfo[i].n, m_shadowOrthoProjInfo[i].f);
		glm::mat4 orthomatrix = glm::ortho(m_shadowOrthoProjInfo[i].l, m_shadowOrthoProjInfo[i].r, m_shadowOrthoProjInfo[i].b, m_shadowOrthoProjInfo[i].t, m_shadowOrthoProjInfo[i].n, m_shadowOrthoProjInfo[i].f);
		glm::mat4 lightvp = orthomatrix * lightview * m_World;
		m_shadow.SetWVP(lightvp);

		for (int i = 0; i < m_Block.size(); i++)
		{
			if (m_Block[i].isparent) continue;
			m_shadow.SetScale(glm::vec4(m_Block[i].scale, 1, m_Block[i].scale, 1));
			m_shadow.SetOffset(glm::vec4(m_Block[i].offset.x, 0, m_Block[i].offset.y, 0));
			glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size() / 3);
		}

		for (int i = 0; i < m_child.size(); i++)
		{
			if (!m_child[i].isparent)
			{
				m_shadow.SetScale(glm::vec4(m_child[i].scale, 1, m_child[i].scale, 1));
				m_shadow.SetOffset(glm::vec4(m_child[i].offset.x, 0, m_child[i].offset.y, 0));
				glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size() / 3);
			}
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}