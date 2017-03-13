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
class ModuleTestScene;
class ModuleSceneManager;
class ModulePrimitives;


class Timer;
class PreciseTimer;

class LightsManager;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	double CalculateAvgFPS();

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
	ModuleAnimation* animations;
	ModuleParticles* particles;
	ModuleSceneManager* sceneManager;
	ModuleTestScene* testScene;
	ModulePrimitives* primitives;


	JSON_Value *configuration;
	Timer *gamestartTimer;
	PreciseTimer *avgTimer;
	PreciseTimer *updateTimer;
	PreciseTimer *performanceTimer;
	PreciseTimer *fpsTimer;
	int frameCountGlobal = 0;
	int frameCountPerSecond = 0;
	float lastFrameMilliseconds = 0;

	// HW info
	SDL_version sdlVersion;
	int CPUCount;
	int CPUCache;
	float systemRAM;
	const char *currentPlatform;

	LightsManager *lightsManager;

private:
	float msByFrame = 0;
	std::list<Module*> modules;


};

extern Application* App;

#endif // __APPLICATION_CPP__