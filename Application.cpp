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
#include "PreciseTimer.h"


using namespace std;

Application::Application()
{
	pTimer = new PreciseTimer();
	pTimer->Start();
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

	//Configurator *configurator = new Configurator();
	//configuration = configurator->LoadConfiguration("config.json");

	LOG("Read precise time after App constructor: %f microseconds", pTimer->Ellapsed());
	LOG("Read precise time after App constructor: %f milliseconds", pTimer->EllapsedInMilliseconds());

	window->ChangeTitle((std::to_string(pTimer->Ellapsed())).c_str());
}

Application::~Application()
{
	for(list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
		RELEASE(*it);
}

bool Application::Init()
{
	bool ret = true;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init(); // we init everything, even if not anabled

	LOG("Read precise time after Init: %f microseconds", pTimer->Ellapsed());
	LOG("Read precise time after Init: %f milliseconds", pTimer->EllapsedInMilliseconds());

	window->ChangeTitle((std::to_string(pTimer->Ellapsed())).c_str());

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
	{
		if((*it)->IsEnabled() == true)
			ret = (*it)->Start();
	}

	LOG("Read precise time after Start: %f microseconds", pTimer->Ellapsed());
	LOG("Read precise time after Start: %f milliseconds", pTimer->EllapsedInMilliseconds());

	window->ChangeTitle((std::to_string(pTimer->Ellapsed())).c_str());

	// Start the first scene --
	return ret;
}

update_status Application::Update()
{
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

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for(list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->CleanUp();

	LOG("Read precise time after CleanUp: %f microseconds", pTimer->Ellapsed());
	LOG("Read precise time after CleanUp: %f milliseconds", pTimer->EllapsedInMilliseconds());
	
	return ret;
}

