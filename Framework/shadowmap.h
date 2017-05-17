#ifndef SHADOWMAP_H
#define SHADOWMAP_H
#include <GL/glew.h>
#include <string>

#define ZERO_MEM(a) memset(a, 0, sizeof(a))
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

class CascadedShadowMapFBO
{
public:
	CascadedShadowMapFBO();

	~CascadedShadowMapFBO();

	bool Init(unsigned int WindowWidth, unsigned int WindowHeight);

	void BindForWriting(int CascadeIndex);

	void BindForReading();

	void Showresult();

private:
	GLuint m_fbo;
	GLuint m_shadowMap[3];
};

#endif
