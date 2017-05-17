#ifndef LIGHT_H
#define LIGHT_H

#define _USE_MATH_DEFINES

#include <glm/glm.hpp>
#include <cmath>
#include <stdio.h>

#define SNPRINTF _snprintf_s
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))
#define ToRadian(x) (float)(((x) * M_PI / 180.0f))
#define ToDegree(x) (float)(((x) * 180.0f / M_PI))

struct BaseLight
{
	glm::vec3 Color;
	float AmbientIntensity;
	float DiffuseIntensity;

	BaseLight()
	{
		Color = glm::vec3(0.0f, 0.0f, 0.0f);
		AmbientIntensity = 0.0f;
		DiffuseIntensity = 0.0f;
	}
};

struct DirectionalLight : public BaseLight
{
	glm::vec3 Direction;

	DirectionalLight()
	{
		Direction = glm::vec3(0.0f, 0.0f, 0.0f);
	}
};

struct PointLight : public BaseLight
{
	glm::vec3 Position;

	struct
	{
		float Constant;
		float Linear;
		float Exp;
	} Attenuation;

	PointLight()
	{
		Position = glm::vec3(0.0f, 0.0f, 0.0f);
		Attenuation.Constant = 1.0f;
		Attenuation.Linear = 0.0f;
		Attenuation.Exp = 0.0f;
	}
};

struct SpotLight : public PointLight
{
	glm::vec3 Direction;
	float Cutoff;

	SpotLight()
	{
		Direction = glm::vec3(0.0f, 0.0f, 0.0f);
		Cutoff = 0.0f;
	}
};

#endif
