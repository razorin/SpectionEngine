#ifndef __MODULEGUI_H__
#define __MODULEGUI_H__

#include "Module.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "IMGUI\imgui.h"
#include "Point.h"
#include<list>

class GameObject;
struct ImGuiTextBuffer;
struct Node;

struct AppConsole {
	ImGuiTextBuffer consoleBuffer;
	bool ScrollToBottom;

	void Clear() { consoleBuffer.clear(); }

	void AddLog(const char* fmt, ...) IM_PRINTFARGS(2)
	{
		va_list args;
		va_start(args, fmt);
		consoleBuffer.appendv(fmt, args);
		consoleBuffer.append("\n");
		va_end(args);
		ScrollToBottom = true;
	}

	bool Draw()
	{
		bool open = true;
		ImGui::SetNextWindowSize(ImVec2((float)(App->window->screen_width * App->window->screen_size), (float)(App->window->screen_height * App->window->screen_size / 4)), ImGuiSetCond_Once);
		ImGui::SetNextWindowPos(ImVec2(0, (float)(App->window->screen_height * App->window->screen_size * 3 / 4)), ImGuiSetCond_Once);
		ImGui::Begin("Console", &open);
		if (ImGui::Button("Clear")) Clear();
		ImGui::Separator();
		ImGui::BeginChild("Scrolling");
		ImGui::TextUnformatted(consoleBuffer.begin());
		if (ScrollToBottom)
			ImGui::SetScrollHere(1.0f);
		ScrollToBottom = false;
		ImGui::EndChild();
		ImGui::End();
		return open;
	}
};

class ModuleGUI :
	public Module
{
public:
	ModuleGUI(const JSON_Object *json = nullptr);
	~ModuleGUI();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();
	bool DrawMainMenuBar();
	bool DrawHWInfoMenu();
	bool DrawPreferencesMenu();
	bool DrawGOHierarchyMenu();
	//bool DrawLightsMenu();
	bool DrawAppInfo();
	bool DrawInspectorMenu();
	void AddFpsLog(float fps);
	void AddMsLog(float ms);

	void RecursiveTreePrint(GameObject & GO);
	void GameObjectSelected(GameObject & GO);

	void ClearSelection();

public:
	AppConsole console;

	bool uiInput = false;

private:
	bool showGUI = true;
	bool resizable = false;

	//Menus
	//File
	bool showHWInfo = false;
	bool showAppInfo = false;
	//Edit
	bool showPreferences = false;
	//Assets
	//GameObject
	bool showGOHierarchy = false;
	//bool showLights = false;
	//Component
	//Window
	bool showInspector = false;
	bool showConsole = true;
	//EndMenus

	int currentDisplayMode = 1;
	int brightness = 1000;
	int minBrightness = 0;
	int maxBrightness = 1000;

	int numFps = 0;
	float fpsLog[100];
	int numMs = 0;
	float msLog[100];

	//Primitive
	fPoint primitivePos = { 0, 0, 0 };
	float primitiveScale;

	//Light
	fPoint lightPos = { 0, 0, 0 };
	int minLightPosition = -100;
	int maxLightPosition = 100;

	ImGuiTreeNodeFlags treeNodeFlags;

	GameObject * selectedGO = nullptr;

	//Inspector
	float matColor[4] = { 1.0f,1.0f,1.0f,1.0f };
	float position[3] = { 0.0f, 0.0f, 0.0f };
	float rotation[3] = { 0.0f, 0.0f, 0.0f };
	float scale[3] = { 0.0f, 0.0f, 0.0f };
};

#endif // __MODULEGUI_H__