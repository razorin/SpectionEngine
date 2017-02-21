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
class ModuleParticles;
class ModuleTestScene;

class Timer;
class PreciseTimer;

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
	ModuleParticles* particles;
	ModuleTestScene* testScene;

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

private:
	float msByFrame = 0;
	std::list<Module*> modules;


};

extern Application* App;

#endif // __APPLICATION_CPP__