#ifndef __APPLICATION_CPP__
#define __APPLICATION_CPP__

#include<list>
#include "Globals.h"
#include "Module.h"
#include "SDL\include\SDL_version.h"

class ModuleRender;
class ModuleWindow;
class ModuleCamera;
class ModuleTextures;
class ModuleInput;
class ModuleAudio;
class ModuleGUI;
class ModuleFadeToBlack;
class ModuleCollision;
class ModuleAnimation;
class ModuleParticles;
class ModuleSceneManager;
class ModulePrimitives;

class LightsManager;
class PTimer;
class ProgramManager;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	void ManageTimersPreUpdate();
	update_status Update();
	void ManageTimersPostUpdate();
	bool CleanUp();

	void LogInTitle(std::string info) const;

public:
	ModuleRender* renderer;
	ModuleWindow* window;
	ModuleCamera* camera;
	ModuleTextures* textures;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleGUI* gui;
	ModuleFadeToBlack* fade;
	ModuleCollision* collision;
	ModuleParticles* particles;
	ModuleSceneManager* sceneManager;
	ModulePrimitives* primitives;
	ModuleAnimation* animator;

	ProgramManager* programManager;

	JSON_Value *configuration;

	

	// HW info
	SDL_version sdlVersion;
	int CPUCount;
	int CPUCache;
	float systemRAM;
	const char *currentPlatform;

	LightsManager *lightsManager;

private:
	PTimer* updateTimer = nullptr;
	PTimer* fpsTimer = nullptr;

	unsigned fps = 0;
	unsigned fpsCount = 0;
	unsigned fpsCap = 0;
	float dt = 0;
	unsigned frameMS = 0;

	unsigned msByFrame = 0;

	std::list<Module*> modules;


};

extern Application* App;

#endif // __APPLICATION_CPP__