#include <string>
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleFadeToBlack.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "Timer.h"
#include "PreciseTimer.h"
#include "Parson.h"


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
	modules.push_back(window = new ModuleWindow(json_object_dotget_object(root, "config.window")));

	modules.push_back(renderer = new ModuleRender(json_object_dotget_object(root, "config.renderer")));
	modules.push_back(textures = new ModuleTextures());
	modules.push_back(audio = new ModuleAudio());

	// Modules to draw on top of game logic
	modules.push_back(collision = new ModuleCollision());
	modules.push_back(particles = new ModuleParticles());
	modules.push_back(fade = new ModuleFadeToBlack());

	JSON_Object* parameters = json_object_dotget_object(root, "config.app");
	fpsCap = (int)json_object_dotget_number(parameters, "fps_cap");

	//Configurator *configurator = new Configurator();
	//configuration = configurator->LoadConfiguration("config.json");

	LOG("Read performance timer after App constructor: %f microseconds", performanceTimer->Ellapsed());
	LOG("Read performance timer after App constructor: %f milliseconds", performanceTimer->EllapsedInMilliseconds());

	window->ChangeTitle((std::to_string(performanceTimer->Ellapsed())).c_str());
}

Application::~Application()
{
	for(list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
		RELEASE(*it);

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

	LOG("Read performance timer after Init: %f microseconds", performanceTimer->Ellapsed());
	LOG("Read performance timer after Init: %f milliseconds", performanceTimer->EllapsedInMilliseconds());

	window->ChangeTitle((std::to_string(performanceTimer->Ellapsed())).c_str());

	performanceTimer->Restart();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
	{
		if((*it)->IsEnabled() == true)
			ret = (*it)->Start();
	}

	LOG("Read performance timer after Start: %f microseconds", performanceTimer->Ellapsed());
	LOG("Read performance timer after Start: %f milliseconds", performanceTimer->EllapsedInMilliseconds());

	window->ChangeTitle((std::to_string(performanceTimer->Ellapsed())).c_str());

	// Start the first scene --
	return ret;
}

update_status Application::Update()
{
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
		LOG("Current FPS: %d", frameCountPerSecond);
		frameCountPerSecond = 0;
		fpsTimer->Restart();
	}
	updateTimer->Restart();
	frameCountGlobal++;
	frameCountPerSecond++;
	
	update_status ret = UPDATE_CONTINUE;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->PreUpdate();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->Update();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->PostUpdate();

	LOG("Read timer since the game started: %d milliseconds", gamestartTimer->Ellapsed());
	LOG("Read update timer: %f microseconds", updateTimer->Ellapsed());
	LOG("Average FPS: %f", CalculateAvgFPS());
	return ret;
}

bool Application::CleanUp()
{
	performanceTimer->Restart();
	bool ret = true;

	for(list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->CleanUp();

	LOG("Read performance timer after CleanUp: %f microseconds", performanceTimer->Ellapsed());
	LOG("Read performance timer after CleanUp: %f milliseconds", performanceTimer->EllapsedInMilliseconds());
	
	return ret;
}

double Application::CalculateAvgFPS()
{
	return (double)frameCountGlobal / (avgTimer->Ellapsed() / 1000000.0f);
}