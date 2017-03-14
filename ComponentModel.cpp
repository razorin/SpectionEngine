#include "ComponentModel.h"

//IMGUI Includes
#include "IMGUI\imconfig.h"
#include "IMGUI\imgui.h"
#include "IMGUI\imgui_impl_sdl_gl3.h"
#include "IMGUI\imgui_internal.h"
#include "IMGUI\stb_rect_pack.h"
#include "IMGUI\stb_textedit.h"
#include "IMGUI\stb_truetype.h"


ComponentModel::ComponentModel(GameObject * container) : Component(container, ComponentType::COMPONENT_TYPE_MESH)
{
}

ComponentModel::~ComponentModel()
{
}

bool ComponentModel::DrawGUI()
{
	if (ImGui::CollapsingHeader("Model", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Model stuff...");
	}
	return true;
}