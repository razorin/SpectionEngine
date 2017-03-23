#ifndef __LIGHTSMANAGER_H__
#define __LIGHTSMANAGER_H__

#include "Light.h"
#include <list>
#include <map>
#include "Glew/include/GL/glew.h"

class LightsManager
{
public:
	LightsManager();
	virtual ~LightsManager();

	bool CleanUp();

	Light* AddLight(LightType type, fPoint position, float4 ambient, float4 diffuse, float4 specular, float constantAttenuation = 1.0f, float linearAttenuation = 0.0f, float quadraticAttenuation = 0.0f);
	Light* AddLight(LightType type, fPoint position, float4 ambient, float4 diffuse, float4 specular, fPoint direction, float exponent = 0.0f, float cutoff = 180.0f, float constantAttenuation = 1.0f, float linearAttenuation = 0.0f, float quadraticAttenuation = 0.0f);
	void Draw();
	std::list<Light*>* GetLights();

	bool EnableLight(ptrdiff_t position);
	bool EnableLight(Light* light);
	bool DisableLight(ptrdiff_t position);
	bool DisableLight(Light* light);

private:
	std::list<Light*> lights;
	std::map<int, GLenum> lightsMap;
};

#endif // __LIGHTSMANAGER_H__