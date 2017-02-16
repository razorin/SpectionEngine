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
	consoleBuffer = new ImGuiTextBuffer();
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
	/*
	ImGui::Text("Hello, world!");

	if (ImGui::Button("Button Name")) {

	}

	ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("Console", NULL);
	consoleBuffer->append("hi\n");
	ImGui::TextUnformatted(consoleBuffer->begin());
	ImGui::End();*/


	Draw("Console");

	

	return UPDATE_CONTINUE;
}

update_status ModuleGUI::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleGUI::CleanUp() {
	Clear();
	ImGui_ImplSdlGL3_Shutdown();

	return true;
}

void ModuleGUI::AddLog(const char* fmt, ...) IM_PRINTFARGS(2)
{
	va_list args;
	va_start(args, fmt);
	consoleBuffer->appendv(fmt, args);
	consoleBuffer->append("\n");
	va_end(args);
	ScrollToBottom = true;
}

void ModuleGUI::Draw(const char* title, bool* p_opened)
{
	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiSetCond_FirstUseEver);
	ImGui::Begin(title, p_opened);
	ImGui::TextUnformatted(consoleBuffer->begin());
	if (ScrollToBottom)
		ImGui::SetScrollHere(1.0f);
	ScrollToBottom = false;
	ImGui::End();
}

void ModuleGUI::Clear()
{
	consoleBuffer->clear();
}