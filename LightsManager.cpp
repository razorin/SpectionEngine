#include "LightsManager.h"
#include "Globals.h"
#include "Glew/include/GL/glew.h"



LightsManager::LightsManager()
{
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

	lights.push_back(new Light(type, position, ambient, diffuse, specular, constantAttenuation,	linearAttenuation, quadraticAttenuation));

	return true;
}

bool LightsManager::AddLight(LightType type, fPoint position, float4 ambient, float4 diffuse, float4 specular,
	fPoint direction, float exponent, float cutoff, float constantAttenuation,
	float linearAttenuation, float quadraticAttenuation) {

	lights.push_back(new Light(type, position, ambient, diffuse, specular, direction, exponent, cutoff, constantAttenuation, linearAttenuation, quadraticAttenuation));

	return true;
}

void LightsManager::RemoveLight(Light *light) {
	lights.remove(light);
}

void LightsManager::Draw() {
	for (std::list<Light*>::iterator it = lights.begin(); it != lights.end(); ++it)
	{
		//TODO: change GL_LIGHT0 to 0-7 depending on the lights count
		glLightfv(GL_LIGHT0, GL_POSITION, (*it)->position);
		glLightfv(GL_LIGHT0, GL_AMBIENT, (*it)->ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, (*it)->diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, (*it)->specular);
		glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, (*it)->constantAttenuation);
		glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, (*it)->linearAttenuation);
		glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, (*it)->quadraticAttenuation);

		if ((*it)->type == LT_SPOTLIGHT_LIGHT) {
			glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, (*it)->direction);
			glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, (*it)->exponent);
			glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, (*it)->cutoff);
		}
	}
}

std::list<Light*> LightsManager::GetLights() {
	return lights;
}