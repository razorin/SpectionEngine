#ifndef __LIGHT_H__
#define __LIGHT_H__
#include "MathGeoLib\include\Math\float4.h"
#include "Point.h"

enum LightType {
	LT_DIRECTIONAL_LIGHT,
	LT_POINT_LIGHT,
	LT_SPOTLIGHT_LIGHT,
	LT_AMBIENT_LIGHT,
	LT_NONE
};

class Light
{
public:
	Light(LightType type, fPoint position, float4 ambient, float4 diffuse, float4 specular, float constantAttenuation = 1.0f, float linearAttenuation = 0.0f, float quadraticAttenuation = 0.0f);
	Light(LightType type, fPoint position, float4 ambient, float4 diffuse, float4 specular, fPoint direction, float exponent = 0.0f, float cutoff = 180.0f, float constantAttenuation = 1.0f, float linearAttenuation = 0.0f, float quadraticAttenuation = 0.0f);
	virtual ~Light();

	void Draw();

public:
	// All lights
	LightType type = LT_NONE;
	float *position = nullptr;
	float *ambient = nullptr;
	float *diffuse = nullptr;
	float *specular = nullptr;
	float constantAttenuation = 1.0f;
	float linearAttenuation = 0.0f;
	float quadraticAttenuation = 0.0f;

	// Spotlight
	float *direction = nullptr;
	float exponent = 0.0f;
	float cutoff = 180.0f;
};

#endif // __LIGHT_H__