#include "ComponentModel.h"

//IMGUI Includes
#include "IMGUI\imconfig.h"
#include "IMGUI\imgui.h"
#include "IMGUI\imgui_impl_sdl_gl3.h"
#include "IMGUI\imgui_internal.h"
#include "IMGUI\stb_rect_pack.h"
#include "IMGUI\stb_textedit.h"
#include "IMGUI\stb_truetype.h"


ComponentModel::ComponentModel(GameObject * container, std::string id) : Component(container, ComponentType::COMPONENT_TYPE_MESH, id)
{
	name = "Model";
}

ComponentModel::~ComponentModel()
{
}

bool ComponentModel::DrawGUI()
{
	std::string headerLabel = name + "##" + id;
	if (ImGui::CollapsingHeader(headerLabel.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		std::string checkboxLabel = "Active##" + id;
		ImGui::Checkbox(checkboxLabel.c_str(), &active);
		ImGui::Text("Model stuff...");
	}
	std::string removeLabel = "Remove Component##" + id;
	if (ImGui::Button(removeLabel.c_str())) {
		toDelete = true;
	}
	return true;
}