#include <string>
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleGUI.h"
#include "ModuleFadeToBlack.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModuleCamera.h"
#include "ModuleTestScene.h"
#include "Timer.h"
#include "PreciseTimer.h"
#include "Parson.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include <assert.h>

using namespace std;

Application::Application()
{
	gamestartTimer = new Timer();
	gamestartTimer->Start();
	avgTimer = new PreciseTimer();
	updateTimer = new PreciseTimer();
	performanceTimer = new PreciseTimer();
	performanceTimer->Start();
	fpsTimer = new PreciseTimer();
	configuration = json_parse_file("config.json");
	JSON_Object *root = json_value_get_object(configuration);
	// Order matters: they will init/start/pre/update/post in this order
	modules.push_back(input = new ModuleInput());
	modules.push_back(camera = new ModuleCamera(json_object_dotget_object(root, "config.camera")));
	modules.push_back(window = new ModuleWindow(json_object_dotget_object(root, "config.window")));

	modules.push_back(renderer = new ModuleRender(json_object_dotget_object(root, "config.renderer")));
	modules.push_back(textures = new ModuleTextures());
	modules.push_back(audio = new ModuleAudio());

	// Modules to draw on top of game logic
	modules.push_back(collision = new ModuleCollision());
	modules.push_back(particles = new ModuleParticles());
	modules.push_back(fade = new ModuleFadeToBlack());

	//Game Modules
	modules.push_back(testScene = new ModuleTestScene(true));

	// UI
	modules.push_back(gui = new ModuleGUI());

	float2 mathGeoLib_test{ 1,2 };

	JSON_Object* parameters = json_object_dotget_object(root, "config.app");
	int fpsCap = (int)json_object_dotget_number(parameters, "fps_cap");
	
	assert(fpsCap > 0);
	msByFrame = (1.f / (float)fpsCap) * 1000;

	//Configurator *configurator = new Configurator();
	//configuration = configurator->LoadConfiguration("config.json");

	//DLOG("Read performance timer after App constructor: %f microseconds", performanceTimer->Ellapsed());
	//DLOG("Read performance timer after App constructor: %f milliseconds", performanceTimer->EllapsedInMilliseconds());
	
	window->ChangeTitle((std::to_string(performanceTimer->Ellapsed())).c_str());
}

Application::~Application()
{
	for(list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
		RELEASE(*it);

	json_value_free(configuration);

	RELEASE(gamestartTimer);
	RELEASE(avgTimer);
	RELEASE(updateTimer);
	RELEASE(performanceTimer);
	RELEASE(fpsTimer);
}

bool Application::Init()
{
	performanceTimer->Restart();
	bool ret = true;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init(); // we init everything, even if not anabled

	//DLOG("Read performance timer after Init: %f microseconds", performanceTimer->Ellapsed());
	//DLOG("Read performance timer after Init: %f milliseconds", performanceTimer->EllapsedInMilliseconds());

	window->ChangeTitle((std::to_string(performanceTimer->Ellapsed())).c_str());

	performanceTimer->Restart();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
	{
		if((*it)->IsEnabled() == true)
			ret = (*it)->Start();
	}

	//DLOG("Read performance timer after Start: %f microseconds", performanceTimer->Ellapsed());
	//DLOG("Read performance timer after Start: %f milliseconds", performanceTimer->EllapsedInMilliseconds());

	window->ChangeTitle((std::to_string(performanceTimer->Ellapsed())).c_str());

	SDL_VERSION(&sdlVersion);
	CPUCount = SDL_GetCPUCount();
	CPUCache = SDL_GetCPUCacheLineSize();
	systemRAM = (float)SDL_GetSystemRAM() * 8 / 1024;
	currentPlatform = SDL_GetPlatform();
	// Start the first scene --
	return ret;
}

update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;

	if (avgTimer->state != TIMER_STATE::TIMER_STARTED) {
		avgTimer->Start();
	}
	if (updateTimer->state != TIMER_STATE::TIMER_STARTED) {
		updateTimer->Start();
	}
	if (fpsTimer->state != TIMER_STATE::TIMER_STARTED) {
		fpsTimer->Start();
	}
	else if (fpsTimer->EllapsedInMilliseconds() >= 1000) {
		//DLOG("Current FPS: %d", frameCountPerSecond);
		gui->AddFpsLog(frameCountPerSecond, 5.0f);
		frameCountPerSecond = 0;
		fpsTimer->Restart();
	}

	auto ellapsedTime = updateTimer->EllapsedInMilliseconds();

	//Delta Time calculated
	float previousFrameTime = lastFrameMilliseconds;
	lastFrameMilliseconds = avgTimer->EllapsedInMilliseconds();
	float dt = lastFrameMilliseconds - previousFrameTime;
	//DLOG("DT: %f milliseconds", dt);

	if (ellapsedTime < this->msByFrame) {
		float beforeDelay = updateTimer->EllapsedInMilliseconds();
		SDL_Delay(msByFrame - ellapsedTime);
		float afterDelay = updateTimer->EllapsedInMilliseconds();
		//DLOG("We waited for %f milliseconds and got back in %f milliseconds", msByFrame - ellapsedTime, afterDelay - beforeDelay);
	}
		
	updateTimer->Restart();
	++frameCountGlobal;
	++frameCountPerSecond;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->PreUpdate(dt);

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->Update(dt);

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->PostUpdate(dt);
	
	//DLOG("Read timer since the game started: %i milliseconds", gamestartTimer->Ellapsed());
	//DLOG("Read update timer: %f microseconds", updateTimer->Ellapsed());
	//DLOG("Average FPS: %f", CalculateAvgFPS());
	
	return ret;
}

bool Application::CleanUp()
{
	performanceTimer->Restart();
	bool ret = true;

	for(list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->CleanUp();

	//DLOG("Read performance timer after CleanUp: %f microseconds", performanceTimer->Ellapsed());
	//DLOG("Read performance timer after CleanUp: %f milliseconds", performanceTimer->EllapsedInMilliseconds());
	
	return ret;
}

double Application::CalculateAvgFPS()
{
	return (double)frameCountGlobal / (avgTimer->Ellapsed() / 1000000.0f);
}