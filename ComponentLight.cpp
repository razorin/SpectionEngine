#include "ComponentLight.h"

//IMGUI Includes
#include "IMGUI\imconfig.h"
#include "IMGUI\imgui.h"
#include "IMGUI\imgui_impl_sdl_gl3.h"
#include "IMGUI\imgui_internal.h"
#include "IMGUI\stb_rect_pack.h"
#include "IMGUI\stb_textedit.h"
#include "IMGUI\stb_truetype.h"



ComponentLight::ComponentLight(GameObject * container, std::string id) : Component(container, ComponentType::COMPONENT_TYPE_LIGHT, id)
{
	name = "Light";
}

ComponentLight::~ComponentLight()
{
}

bool ComponentLight::DrawGUI()
{
	std::string headerLabel = name + "##" + id;
	if (ImGui::CollapsingHeader(headerLabel.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		std::string checkboxLabel = "Active##" + id;
		ImGui::Checkbox(checkboxLabel.c_str(), &active);
		ImGui::Text("Light stuff...");
	}
	std::string removeLabel = "Remove Component##" + id;
	if (ImGui::Button(removeLabel.c_str())) {
		toDelete = true;
	}
	return true;
}