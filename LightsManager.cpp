#include "LightsManager.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleGUI.h"
#include "Glew/include/GL/glew.h"



LightsManager::LightsManager()
{
	lightsMap[0] = GL_LIGHT0;
	lightsMap[1] = GL_LIGHT1;
	lightsMap[2] = GL_LIGHT2;
	lightsMap[3] = GL_LIGHT3;
	lightsMap[4] = GL_LIGHT4;
	lightsMap[5] = GL_LIGHT5;
	lightsMap[6] = GL_LIGHT6;
	lightsMap[7] = GL_LIGHT7;
}


LightsManager::~LightsManager()
{
}

bool LightsManager::CleanUp() {
	for (std::list<Light*>::iterator it = lights.begin(); it != lights.end(); ++it)
	{
		RELEASE(*it);
	}
	return true;
}

bool LightsManager::AddLight(LightType type, fPoint position, float4 ambient, float4 diffuse, float4 specular,
	float constantAttenuation, float linearAttenuation, float quadraticAttenuation) {

	if (lights.size() < MAXLIGHTS) {
		lights.push_back(new Light(type, position, ambient, diffuse, specular, constantAttenuation, linearAttenuation, quadraticAttenuation));
		EnableLight(lights.size() - 1);
	}

	return true;
}

bool LightsManager::AddLight(LightType type, fPoint position, float4 ambient, float4 diffuse, float4 specular,
	fPoint direction, float exponent, float cutoff, float constantAttenuation,
	float linearAttenuation, float quadraticAttenuation) {

	if (lights.size() < MAXLIGHTS) {
		lights.push_back(new Light(type, position, ambient, diffuse, specular, direction, exponent, cutoff, constantAttenuation, linearAttenuation, quadraticAttenuation));
		EnableLight(lights.size() - 1);
	}

	return true;
}

void LightsManager::Draw() {
	for (std::list<Light*>::iterator it = lights.begin(); it != lights.end(); ++it)
	{
		auto itPosition = std::distance(lights.begin(), it);
		GLenum lightNumber = lightsMap.find(itPosition)->second;
		glLightfv(lightNumber, GL_POSITION, (*it)->position);
		glLightfv(lightNumber, GL_AMBIENT, (*it)->ambient);
		glLightfv(lightNumber, GL_DIFFUSE, (*it)->diffuse);
		glLightfv(lightNumber, GL_SPECULAR, (*it)->specular);
		glLightf(lightNumber, GL_CONSTANT_ATTENUATION, (*it)->constantAttenuation);
		glLightf(lightNumber, GL_LINEAR_ATTENUATION, (*it)->linearAttenuation);
		glLightf(lightNumber, GL_QUADRATIC_ATTENUATION, (*it)->quadraticAttenuation);

		if ((*it)->type == LT_SPOTLIGHT_LIGHT) {
			glLightfv(lightNumber, GL_SPOT_DIRECTION, (*it)->direction);
			glLightf(lightNumber, GL_SPOT_EXPONENT, (*it)->exponent);
			glLightf(lightNumber, GL_SPOT_CUTOFF, (*it)->cutoff);
		}
	}
}

std::list<Light*>* LightsManager::GetLights() {
	return &lights;
}

bool LightsManager::EnableLight(ptrdiff_t position) {
	GLenum lightNumber = lightsMap.find(position)->second;
	glEnable(lightNumber);
	return true;
}

bool LightsManager::DisableLight(ptrdiff_t position) {
	GLenum lightNumber = lightsMap.find(position)->second;
	glDisable(lightNumber);
	return true;
}