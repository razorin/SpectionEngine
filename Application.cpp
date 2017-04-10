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

#include "PTimer.h"
#include "LightsManager.h"
#include "Parson.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include <assert.h>
#include "SDL\include\SDL_cpuinfo.h"
#include "SDL\include\SDL_timer.h"

using namespace std;

Application::Application()
{
	updateTimer = new PTimer();
	fpsTimer = new PTimer();
	fpsTimer->Start();

	configuration = json_parse_file("config.json");
	JSON_Object *root = json_value_get_object(configuration);

	JSON_Object* parameters = json_object_dotget_object(root, "config.app");
	fpsCap = (unsigned)json_object_dotget_number(parameters, "fps_cap");

	assert(fpsCap > 0);
	msByFrame = 1000 / fpsCap;


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

	modules.push_back(animator = new ModuleAnimation());

	lightsManager = new LightsManager();
}

Application::~Application()
{
	for(list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
		RELEASE(*it);

	json_value_free(configuration);

	RELEASE(lightsManager);


	RELEASE(updateTimer);
	RELEASE(fpsTimer);
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

void Application::ManageTimersPreUpdate()
{
	dt = (float)updateTimer->GetTimeUS() / 1000.0f;
	updateTimer->Restart();
}

update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;

	ManageTimersPreUpdate();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->PreUpdate(dt);

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->Update(dt);

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->PostUpdate(dt);
	
	
	ManageTimersPostUpdate();

	return ret;
}

void Application::ManageTimersPostUpdate()
{
	fpsCount++;

	if (fpsTimer->GetTimeUS() >= 1000000)
	{
		fps = fpsCount;
		fpsCount = 0;
		fpsTimer->Restart();
	}

	//We cant work with US here because sdlDelay works with ms
	frameMS = updateTimer->GetTimeMS();
	if (frameMS < msByFrame)
	{
		SDL_Delay(msByFrame - frameMS);
	}

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


void Application::LogInTitle(string info) const
{
	window->ChangeTitle(info.c_str());
}
