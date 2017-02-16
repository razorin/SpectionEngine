#include "ModuleGUI.h"
#include "Application.h"
#include "ModuleWindow.h"

//IMGUI Includes
#include "IMGUI\imconfig.h"
#include "IMGUI\imgui.h"
#include "IMGUI\imgui_impl_sdl_gl3.h"
#include "IMGUI\imgui_internal.h"
#include "IMGUI\stb_rect_pack.h"
#include "IMGUI\stb_textedit.h"
#include "IMGUI\stb_truetype.h"


ModuleGUI::ModuleGUI(const JSON_Object *json) : Module(json)
{
}


ModuleGUI::~ModuleGUI()
{
}

bool ModuleGUI::Init() {
	//gl3winit();
	ImGui_ImplSdlGL3_Init(App->window->window);
	
	return true;
}

update_status ModuleGUI::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleGUI::Update(float dt)
{
	ImGui::Text("Hello, world!");

	if (ImGui::Button("Button Name")) {

	}

	return UPDATE_CONTINUE;
}

update_status ModuleGUI::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleGUI::CleanUp() {
	ImGui_ImplSdlGL3_Shutdown();

	return true;
}