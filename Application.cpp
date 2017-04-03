#include <string>
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleGUI.h"
#include "ModuleAnimation.h"
#include "ModuleCamera.h"
#include "ModuleSceneManager.h"
#include "ModulePrimitives.h"
#include "Scene.h"
#include "ComponentTransform.h"

#include "Timer.h"
#include "PreciseTimer.h"
#include "LightsManager.h"
#include "Parson.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include <assert.h>
#include "SDL\include\SDL_cpuinfo.h"
#include "SDL\include\SDL_timer.h"

using namespace std;

Application::Application()
{
	avgTimer = new PreciseTimer();
	updateTimer = new PreciseTimer();
	fpsTimer = new PreciseTimer();
	gameTimer = new PreciseTimer();
	gameTimer->Stop();

	configuration = json_parse_file("config.json");
	JSON_Object *root = json_value_get_object(configuration);
	// Order matters: they will init/start/pre/update/post in this order
	modules.push_back(input = new ModuleInput());
	modules.push_back(camera = new ModuleCamera(json_object_dotget_object(root, "config.camera")));
	modules.push_back(window = new ModuleWindow(json_object_dotget_object(root, "config.window")));

	modules.push_back(gui = new ModuleGUI());
	modules.push_back(renderer = new ModuleRender(json_object_dotget_object(root, "config.renderer")));
	modules.push_back(textures = new ModuleTextures());
	modules.push_back(audio = new ModuleAudio());
	modules.push_back(primitives = new ModulePrimitives());

	//Game Modules
	modules.push_back(sceneManager = new ModuleSceneManager(nullptr,true));

	//TODO module animation should go before module render. Animate things before render them!
	modules.push_back(animator = new ModuleAnimation());

	lightsManager = new LightsManager();

	JSON_Object* parameters = json_object_dotget_object(root, "config.app");
	int fpsCap = (int)json_object_dotget_number(parameters, "fps_cap");
	
	assert(fpsCap > 0);
	msByFrame = (1.f / (float)fpsCap) * 1000;
}

Application::~Application()
{
	for(list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
		RELEASE(*it);

	json_value_free(configuration);

	RELEASE(avgTimer);
	RELEASE(updateTimer);
	RELEASE(fpsTimer);
	RELEASE(gameTimer);

	RELEASE(lightsManager);
}

bool Application::Init()
{
	bool ret = true;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init(); // we init everything, even if not anabled

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
	{
		if((*it)->IsEnabled() == true)
			ret = (*it)->Start();
	}

	// Gather hardware settings
	SDL_VERSION(&sdlVersion);
	CPUCount = SDL_GetCPUCount();
	CPUCache = SDL_GetCPUCacheLineSize();
	systemRAM = (float)SDL_GetSystemRAM() * 8 / 1024;
	currentPlatform = SDL_GetPlatform();
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
		gui->AddFpsLog(frameCountPerSecond);
		window->ChangeTitle((std::to_string(frameCountPerSecond)).c_str());
		frameCountPerSecond = 0;
		fpsTimer->Restart();
	}

	auto ellapsedTime = updateTimer->EllapsedInMilliseconds();

	//Delta Time calculated
	float previousFrameTime = lastFrameMilliseconds;
	lastFrameMilliseconds = avgTimer->EllapsedInMilliseconds();
	float dt = lastFrameMilliseconds - previousFrameTime;
	assert(dt >= 0);
	//DLOG("DT: %f milliseconds", dt);
	//window->ChangeTitle((std::to_string(dt)).c_str());

	if (ellapsedTime < this->msByFrame) {
		gui->AddMsLog(msByFrame);
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

	//DLOG("Average FPS: %f", CalculateAvgFPS());
	
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for(list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->CleanUp();

	lightsManager->CleanUp();
	
	return ret;
}

void Application::Play()
{
	if (state != APP_STATE::APP_PLAY) {
		gameTimer->Start();
		state = APP_PLAY;
		DLOG("PLAY");
		//"Save" the gameobjects transforms
		sceneManager->Play();
	}
	
}

void Application::Pause()
{
	if (state == APP_STATE::APP_PLAY) {
		gameTimer->Pause();
		state = APP_PAUSE;
		DLOG("PAUSE");
	}
}

void Application::Stop()
{
	if (state != APP_STATE::APP_STOP) {
		gameTimer->Stop();
		state = APP_STOP;
		DLOG("STOP");
		//"Load" the gameobjects transforms
		sceneManager->Stop();
	}
}

double Application::CalculateAvgFPS()
{
	return (double)frameCountGlobal / (avgTimer->Ellapsed() / 1000000.0f);
}