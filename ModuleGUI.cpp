#include "ModuleGUI.h"
#include "Utils.h"
#include "Application.h"
#include "ModuleWindow.h"
#include<list>

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
	/*
	ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("Help Menu");
	if (ImGui::Button("Open Google")) {
		utils::RequestBrowser("www.google.com");
	}
	ImGui::End();
	*/

	ImGui::Begin("Main Menu");
	ImGui::SetWindowSize(ImVec2(400, 200), ImGuiSetCond_FirstUseEver);
	//ImGui::SetWindowPos(ImVec2(50, 200), ImGuiSetCond_FirstUseEver);
	if (ImGui::BeginMenu("Help")) {
		if (ImGui::MenuItem("Code")) {
			utils::RequestBrowser("https://github.com/razorin/SpectionEngine/");
		}
		if (ImGui::MenuItem("Documentation")) {
			utils::RequestBrowser("https://github.com/razorin/SpectionEngine/wiki");
		}
		if (ImGui::MenuItem("Download Latest")) {
			utils::RequestBrowser("https://github.com/razorin/SpectionEngine/releases");
		}
		if (ImGui::MenuItem("Report a bug")) {
			utils::RequestBrowser("https://github.com/razorin/SpectionEngine/releases");
		}
		ImGui::EndMenu();
	}
	ImGui::End();

	console.Draw("Console");

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

void ModuleGUI::AddFpsLog(float fps, float ms) {
	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiSetCond_FirstUseEver);
	if (fpsLog.size() == 100) {
		//shift values to the left
		
		//Add last values
		fpsLog.push_back(fps);
		msLog.push_back(ms);
	}
	else {
		fpsLog.push_back(fps);
		msLog.push_back(ms);
		numFps++;
		numMs++;
	}
	//Draw
	/*char title[25];
	sprintf_s(title, 25, "Framerate %.1f", fpsLog[numFps-1]);
	ImGui::PlotHistogram("##framerate",&fpsLog[0],numFps,0,title,0.0f,100.0f, ImVec2(310, 100));
	sprintf_s(title, 25, "Milliseconds %.1f", ms_log[numMs - 1]);
	ImGui::PlotHistogram("##milliseconds", &ms_log[0], numMs, 0, title, 0.0f, 40.0f, ImVec2(310, 100));
	ImGui::End();*/
}