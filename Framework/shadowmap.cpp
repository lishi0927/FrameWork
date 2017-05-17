#include "shadowmap.h"
#include "texture.hpp"
#include <cassert>

CascadedShadowMapFBO::CascadedShadowMapFBO()
{
	m_fbo = 0;
	ZERO_MEM(m_shadowMap);
}

CascadedShadowMapFBO::~CascadedShadowMapFBO()
{
	if (m_fbo != 0) {
		glDeleteFramebuffers(1, &m_fbo);
	}

	glDeleteTextures(ARRAY_SIZE_IN_ELEMENTS(m_shadowMap), m_shadowMap);
}

bool CascadedShadowMapFBO::Init(unsigned int WindowWidth, unsigned int WindowHeight)
{
	// Create the FBO
	glGenFramebuffers(1, &m_fbo);

	// Create the depth buffer    
	glGenTextures(ARRAY_SIZE_IN_ELEMENTS(m_shadowMap), m_shadowMap);

	for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_shadowMap); i++) {
		glBindTexture(GL_TEXTURE_2D, m_shadowMap[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, WindowWidth, WindowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadowMap[0], 0);

	// Disable writes to the color buffer
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		return false;
	}

	return true;
}


void CascadedShadowMapFBO::BindForWriting(int CascadeIndex)
{
	assert(CascadeIndex < ARRAY_SIZE_IN_ELEMENTS(m_shadowMap));
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadowMap[CascadeIndex], 0);
}


void CascadedShadowMapFBO::BindForReading()
{
	glActiveTexture(GL_TEXTURE12);
	glBindTexture(GL_TEXTURE_2D, m_shadowMap[0]);

	glActiveTexture(GL_TEXTURE13);
	glBindTexture(GL_TEXTURE_2D, m_shadowMap[1]);

	glActiveTexture(GL_TEXTURE14);
	glBindTexture(GL_TEXTURE_2D, m_shadowMap[2]);
}
void CascadedShadowMapFBO::Showresult()
{
	static float data[1024 * 720];
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glReadPixels(0, 0, 1024, 720, GL_DEPTH_COMPONENT, GL_FLOAT, (void*)&data);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	SaveImage(data, 1024, 720, "test_lod.png");
}