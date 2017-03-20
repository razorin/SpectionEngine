#include "ComponentScript.h"

//IMGUI Includes
#include "IMGUI\imconfig.h"
#include "IMGUI\imgui.h"
#include "IMGUI\imgui_impl_sdl_gl3.h"
#include "IMGUI\imgui_internal.h"
#include "IMGUI\stb_rect_pack.h"
#include "IMGUI\stb_textedit.h"
#include "IMGUI\stb_truetype.h"


ComponentScript::ComponentScript(GameObject * container, std::string id) : Component(container, ComponentType::COMPONENT_TYPE_SCRIPT, id)
{
}

ComponentScript::~ComponentScript()
{
}

bool ComponentScript::DrawGUI()
{
	if (ImGui::CollapsingHeader("Script", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Script stuff...");
	}
	return true;
}