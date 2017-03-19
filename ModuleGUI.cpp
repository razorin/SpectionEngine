#include "Globals.h"
#include "ModuleGUI.h"
#include "Utils.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleCamera.h"
#include "LightsManager.h"
#include "Light.h"
#include "ModuleSceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "SDL\include\SDL_version.h"
#include "Point.h"
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
	ImGui_ImplSdlGL3_Init(App->window->window);
	return true;
}

update_status ModuleGUI::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleGUI::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;
	ImGuiIO& io = ImGui::GetIO();
	uiInput = io.WantCaptureMouse;

	if (showGUI) {
		if (!DrawMainMenuBar()) {
			ret = UPDATE_STOP;
		}

		if (showHWInfo) {
			showHWInfo = DrawHWInfoMenu();
		}

		if (showAppInfo) {
			showAppInfo = DrawAppInfo();
		}

		if (showPreferences) {
			showPreferences = DrawPreferencesMenu();
		}

		if (showGOHierarchy) {
			showGOHierarchy = DrawGOHierarchyMenu();
		}

		if (showLights) {
			showLights = DrawLightsMenu();
		}

		if (showInspector) {
			showInspector = DrawInspectorMenu();
		}

		if (showConsole) {
			showConsole = console.Draw();
		}
	}

	//transform->DrawGUI();
	return ret;
}

update_status ModuleGUI::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleGUI::CleanUp() {
	ImGui_ImplSdlGL3_Shutdown();

	return true;
}

bool ModuleGUI::DrawMainMenuBar() {
	bool ret = false;
	if (ImGui::BeginMainMenuBar())
	{
		ret = true;
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Hardware Information")) { showHWInfo = true; }
			if (ImGui::MenuItem("Application Information")) { showAppInfo = true; }
			ImGui::Separator();
			if (ImGui::MenuItem("Exit")) { ret = false; }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			//if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			//if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
			//ImGui::Separator();
			//if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			//if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			//if (ImGui::MenuItem("Paste", "CTRL+V")) {}
			//ImGui::Separator();
			if (ImGui::MenuItem("Preferences")) { showPreferences = true; }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Assets"))
		{
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("GameObject"))
		{
			if (ImGui::MenuItem("Create Empty")) { App->sceneManager->getCurrentScene()->AddGameObject(OT_EMPTY); }
			if (ImGui::MenuItem("GameObjects Hierarchy")) { showGOHierarchy = true; }
			if (ImGui::MenuItem("Lights")) { showLights = true; }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Component"))
		{
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Window"))
		{
			if (ImGui::MenuItem("Inspector")) { showInspector = true; }
			ImGui::Separator();
			if (ImGui::MenuItem("Console")) { showConsole = true; }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Website")) {
				utils::RequestBrowser("https://github.com/razorin/SpectionEngine/");
			}
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
				utils::RequestBrowser("https://github.com/razorin/SpectionEngine/issues");
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("About"))
		{
			if (ImGui::MenuItem("Spection Engine")) {
				utils::RequestBrowser("https://github.com/razorin/SpectionEngine/");
			}
			ImGui::Text("Description of the Engine");
			ImGui::Text("Authors:");
			if (ImGui::MenuItem("- Roberto Azorin")) {
				utils::RequestBrowser("https://github.com/razorin/");
			}
			if (ImGui::MenuItem("- Sergi Ledesma")) {
				utils::RequestBrowser("https://github.com/SergiLedesma");
			}
			if (ImGui::MenuItem("- Pau Mamano")) {
				utils::RequestBrowser("https://github.com/kamtxatka");
			}
			if (ImGui::MenuItem("- Manuel Mendez")) {
				utils::RequestBrowser("https://github.com/troyanlp");
			}
			ImGui::Text("Libraries used:");
			if (ImGui::MenuItem("- Glew 2.0.0")) {
				utils::RequestBrowser("http://glew.sourceforge.net/");
			}
			if (ImGui::MenuItem("- DevIL 1.8.0")) {
				utils::RequestBrowser("http://openil.sourceforge.net/");
			}
			if (ImGui::MenuItem("- Assimp 3.3.1")) {
				utils::RequestBrowser("http://www.assimp.org/");
			}
			if (ImGui::MenuItem("- ImGui")) {
				utils::RequestBrowser("https://github.com/ocornut/imgui");
			}
			if (ImGui::MenuItem("MIT License")) {
				utils::RequestBrowser("https://github.com/razorin/SpectionEngine/blob/master/LICENSE");
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	return ret;
}

bool ModuleGUI::DrawHWInfoMenu() {
	bool open = true;
	ImGui::SetNextWindowSize(ImVec2(300, 120), ImGuiSetCond_Once);
	ImGui::SetNextWindowPos(ImVec2(0, 400), ImGuiSetCond_Once);
	ImGui::Begin("Hardware Info", &open);
	ImGui::Text("SDL Version:");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.0f, 1.0f), "%d.%d.%d", App->sdlVersion.major, App->sdlVersion.minor, App->sdlVersion.patch);
	ImGui::Separator();
	ImGui::Text("CPUs:");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.0f, 1.0f), "%d (Cache: %dkb)", App->CPUCount, App->CPUCache);
	ImGui::Text("System RAM:");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.0f, 1.0f), "%.2fGb", App->systemRAM);
	ImGui::Separator();
	ImGui::Text("Current Platform:");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.0f, 1.0f), "%s", App->currentPlatform);
	ImGui::End();
	return open;
}

bool ModuleGUI::DrawPreferencesMenu() {
	bool open = true;
	ImGui::SetNextWindowSize(ImVec2(400, 150), ImGuiSetCond_Once);
	ImGui::SetNextWindowPos(ImVec2(0, 200), ImGuiSetCond_Once);
	ImGui::Begin("Preferences", &open);
	const char* items[] = { "Fullscreen", "Borderless", "Fullscreen Windowed" };
	if (ImGui::Combo("Display", &currentDisplayMode, items, IM_ARRAYSIZE(items))) {
		App->window->SetDisplayMode(static_cast<DisplayMode>(currentDisplayMode));
	}
	if (ImGui::SliderInt("Brightness", &brightness, minBrightness, maxBrightness)) {
		App->window->SetBrightness(brightness);
	}
	if (ImGui::Checkbox("Invert X-Axis", &App->camera->invertXAxis)) {}
	ImGui::SameLine();
	if (ImGui::Checkbox("Invert Y-Axis", &App->camera->invertYAxis)) {}
	ImGui::End();
	return open;
}

bool ModuleGUI::DrawGOHierarchyMenu() {
	if (App->sceneManager->getCurrentScene() != nullptr)
	{
		GameObject* root = App->sceneManager->getCurrentScene()->root;
		bool open = true;

		ImGui::SetNextWindowSize(ImVec2(600, 200), ImGuiSetCond_Once);
		ImGui::SetNextWindowPos(ImVec2(300, 200), ImGuiSetCond_Once);
		treeNodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
		ImGui::Begin("GameObjects Hierarchy", &open);
		bool treeNode = ImGui::TreeNodeEx(root->name.c_str(), treeNodeFlags);

		if (ImGui::IsItemClicked()) {
			GameObjectSelected(*root);
		}
		if (treeNode) {
			for (auto it = root->childs.begin(); it != root->childs.end(); ++it)
			{
				RecursiveTreePrint(**it);
			}
			ImGui::TreePop();
		}
		ImGui::End();
		return open;
	}
	else {
		return false;
	}
}

void ModuleGUI::RecursiveTreePrint(GameObject & GO)
{
	if (!GO.name.empty()) {
		bool treeNode = ImGui::TreeNodeEx(GO.name.c_str(), treeNodeFlags);
		if (ImGui::IsItemClicked()) { GameObjectSelected(GO); }
		if (treeNode) {
			for (auto it = GO.childs.begin(); it != GO.childs.end(); ++it)
			{
				RecursiveTreePrint(**it);
			}
			ImGui::TreePop();
		}
	}
}

void ModuleGUI::GameObjectSelected(GameObject & GO) {
	console.AddLog("%s Selected", GO.name.c_str());
	selectedGO = &GO;
}

bool ModuleGUI::DrawLightsMenu() {
	bool open = true;
	const char* items[] = { "DIRECTIONAL LIGHT", "POINT LIGHT", "SPOTLIGHT", "AMBIENTLIGHT" };
	ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiSetCond_Once);
	ImGui::SetNextWindowPos(ImVec2(300, 200), ImGuiSetCond_Once);
	ImGui::Begin("Lights", &open);
	std::list<Light*>* currentLights = App->lightsManager->GetLights();
	for (std::list<Light*>::iterator it = currentLights->begin(); it != currentLights->end(); )
	{
		auto itPosition = std::distance(currentLights->begin(), it);
		std::string tempString = "Light" + std::to_string(itPosition);
		const char * headerLabel = tempString.c_str();
		bool hasBeenRemoved = false;
		if (ImGui::CollapsingHeader(headerLabel))
		{
			int newType = (*it)->type;
			ImGui::Text("Light Type %d", (*it)->type);
			// Generic strings
			std::string typeName = "Type " + std::to_string(itPosition);
			std::string position = "Position " + std::to_string(itPosition);
			std::string diffuse = "Diffuse " + std::to_string(itPosition);
			std::string ambient = "Ambient " + std::to_string(itPosition);
			std::string specular = "Specular " + std::to_string(itPosition);
			std::string direction = "Direction " + std::to_string(itPosition);
			// Extra strings
			std::string exponent = "Exponent " + std::to_string(itPosition);
			std::string cutoff = "Cutoff " + std::to_string(itPosition);
			std::string constantAttenuation = "Constant Attenuation " + std::to_string(itPosition);
			std::string linearAttenuation = "Linear Attenuation " + std::to_string(itPosition);
			std::string quadraticAttenuation = "Quadratic Attenuation " + std::to_string(itPosition);
			if (ImGui::Combo(typeName.c_str(), &newType, items, IM_ARRAYSIZE(items))) {
				(*it)->type = static_cast<LightType>(newType);
			}
			if ((*it)->type == LT_DIRECTIONAL_LIGHT) {
				ImGui::InputFloat3(direction.c_str(), (*it)->position);
			}
			else {
				ImGui::InputFloat3(position.c_str(), (*it)->position);
			}
			ImGui::ColorEdit3(diffuse.c_str(), (*it)->diffuse);
			ImGui::ColorEdit3(ambient.c_str(), (*it)->ambient);
			ImGui::ColorEdit3(specular.c_str(), (*it)->specular);
			if ((*it)->type == LT_SPOTLIGHT_LIGHT) {
				ImGui::InputFloat3(direction.c_str(), (*it)->direction);
				ImGui::InputFloat(exponent.c_str(), &(*it)->exponent);
				ImGui::InputFloat(cutoff.c_str(), &(*it)->cutoff);
			}
			if (ImGui::TreeNode("More options"))
			{
				ImGui::InputFloat(constantAttenuation.c_str(), &(*it)->constantAttenuation);
				ImGui::InputFloat(linearAttenuation.c_str(), &(*it)->linearAttenuation);
				ImGui::InputFloat(quadraticAttenuation.c_str(), &(*it)->quadraticAttenuation);
				ImGui::TreePop();
			}
			std::string remove = "Remove Light " + std::to_string(itPosition);
			if (ImGui::Button(remove.c_str())) {
				hasBeenRemoved = true;
				App->lightsManager->DisableLight(itPosition);
				RELEASE(*it);
				it = currentLights->erase(it);
			}
		}
		if (!hasBeenRemoved) {
			++it;
		}
	}
	if (currentLights->size() < MAXLIGHTS) {
		if (ImGui::Button("New Light")) { App->lightsManager->AddLight(LT_POINT_LIGHT, { 0.0f, 5.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }); }
	}

	ImGui::End();
	return open;
}

bool ModuleGUI::DrawAppInfo() {
	bool open = true;
	float menuWidth = 400;
	ImGui::SetNextWindowSize(ImVec2(menuWidth, 300), ImGuiSetCond_Once);
	ImGui::SetNextWindowPos(ImVec2(800, 20), ImGuiSetCond_Once);
	ImGui::Begin("Application Information", &open);
	ImGui::Text("Application Name: %s", App->window->title);
	char title[25];
	sprintf_s(title, 25, "Framerate %.1f", fpsLog[numFps - 1]);
	ImGui::PlotHistogram("##framerate", &fpsLog[0], numFps, 0, title, 0.0f, 100.0f, ImVec2(menuWidth - 30, 100));
	sprintf_s(title, 25, "Milliseconds %.1f", msLog[numMs - 1]);
	ImGui::PlotHistogram("##milliseconds", &msLog[0], numMs, 0, title, 0.0f, 40.0f, ImVec2(menuWidth - 30, 100));
	ImGui::End();
	return open;
}

bool ModuleGUI::DrawInspectorMenu() {
	bool open = true;
	float menuWidth = 550;
	ImGui::SetNextWindowSize(ImVec2(menuWidth, 750), ImGuiSetCond_Once);
	float menuPosY = 19;
	ImGui::SetNextWindowPos(ImVec2(App->window->screen_width - menuWidth, menuPosY), ImGuiSetCond_Once);
	ImGui::Begin("Inspector", &open);
	if (selectedGO != nullptr) {
		selectedGO->DrawGUIPanel();
	}
	ImGui::End();
	return open;
}

void ModuleGUI::AddFpsLog(float fps) {
	if (numFps == 100) {
		// Shift values to the left
		for (int j = 0; j < numFps - 1; j++) {
			fpsLog[j] = fpsLog[j + 1];
		}
		// Add last values
		fpsLog[numFps - 1] = fps;
	}
	else {
		fpsLog[numFps] = fps;
		++numFps;
	}

}

void ModuleGUI::AddMsLog(float ms) {
	if (numMs == 100) {
		// Shift values to the left
		for (int j = 0; j < numMs - 1; j++) {
			msLog[j] = msLog[j + 1];
		}
		// Add last value
		msLog[numMs - 1] = ms;
	}
	else {
		msLog[numMs] = ms;
		++numMs;
	}
}