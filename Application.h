#ifndef __APPLICATION_CPP__
#define __APPLICATION_CPP__

#include<list>
#include "Globals.h"
#include "Module.h"

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleAudio;
class ModuleFadeToBlack;
class ModuleCollision;
class ModuleParticles;

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
	ModuleTextures* textures;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleFadeToBlack* fade;
	ModuleCollision* collision;
	ModuleParticles* particles;

	JSON_Value *configuration;
	Timer *gamestartTimer;
	PreciseTimer *avgTimer;
	PreciseTimer *updateTimer;
	PreciseTimer *performanceTimer;
	PreciseTimer *fpsTimer;
	int frameCountGlobal = 0;
	int frameCountPerSecond = 0;

private:

	std::list<Module*> modules;


};

extern Application* App;

#endif // __APPLICATION_CPP__