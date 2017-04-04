#include "Light.h"
#include "Globals.h"
#include "Glew/include/GL/glew.h"

Light::Light(LightType type, fPoint position, float4 ambient, float4 diffuse, float4 specular,
	float constantAttenuation, float linearAttenuation, float quadraticAttenuation)
{
	this->type = type;
	float fourthPositionValue = 1.0f;
	if (type == LT_DIRECTIONAL_LIGHT) {
		fourthPositionValue = 0.0f;
	}
	this->position = new float[4]{ position.x, position.y, position.z, fourthPositionValue };
	this->ambient = new float[4]{ ambient.x, ambient.y, ambient.z, ambient.w };
	this->diffuse = new float[4]{ diffuse.x, diffuse.y, diffuse.z, diffuse.w };
	this->specular = new float[4]{ specular.x, specular.y, specular.z, specular.w };
	this->direction = new float[3]{ 0, 0, 0 };
	this->exponent = 0;
	this->cutoff = 180.0f;
	this->constantAttenuation = constantAttenuation;
	this->linearAttenuation = linearAttenuation;
	this->quadraticAttenuation = quadraticAttenuation;
}

Light::Light(LightType type, fPoint position, float4 ambient, float4 diffuse, float4 specular,
	fPoint direction, float exponent, float cutoff, float constantAttenuation, float linearAttenuation, float quadraticAttenuation)
{
	this->type = type;
	float fourthPositionValue = 1.0f;
	this->position = new float[4]{ position.x, position.y, position.z, fourthPositionValue };
	this->ambient = new float[4]{ ambient.x, ambient.y, ambient.z, ambient.w };
	this->diffuse = new float[4]{ diffuse.x, diffuse.y, diffuse.z, diffuse.w };
	this->specular = new float[4]{ specular.x, specular.y, specular.z, specular.w };
	this->direction = new float[3]{ direction.x, direction.y, direction.z };
	this->exponent = exponent;
	this->cutoff = cutoff;
	this->constantAttenuation = constantAttenuation;
	this->linearAttenuation = linearAttenuation;
	this->quadraticAttenuation = quadraticAttenuation;
}

Light::Light(Light * light) : type(light->type), constantAttenuation(light->constantAttenuation),
linearAttenuation(light->linearAttenuation), quadraticAttenuation(light->quadraticAttenuation),
exponent(light->exponent), cutoff(light->cutoff)
{
	if (light->position != nullptr) {
		position = new float[4];
		std::copy(light->position, light->position + 4, position);
	}
	if (light->ambient != nullptr) {
		ambient = new float[4];
		std::copy(light->ambient, light->ambient + 4, ambient);
	}
	if (light->diffuse != nullptr) {
		diffuse = new float[4];
		std::copy(light->diffuse, light->diffuse + 4, diffuse);
	}
	if (light->specular != nullptr) {
		specular = new float[4];
		std::copy(light->specular, light->specular + 4, specular);
	}
	if (light->direction != nullptr) {
		direction = new float[3];
		std::copy(light->direction, light->direction + 3, direction);
	}
}

Light::~Light()
{
	RELEASE(position);
	RELEASE(ambient);
	RELEASE(diffuse);
	RELEASE(specular);
	RELEASE(direction);	
}