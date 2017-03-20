#include "ComponentMaterial.h"

//IMGUI Includes
#include "IMGUI\imconfig.h"
#include "IMGUI\imgui.h"
#include "IMGUI\imgui_impl_sdl_gl3.h"
#include "IMGUI\imgui_internal.h"
#include "IMGUI\stb_rect_pack.h"
#include "IMGUI\stb_textedit.h"
#include "IMGUI\stb_truetype.h"


ComponentMaterial::ComponentMaterial(GameObject * container, std::string id) : Component(container, ComponentType::COMPONENT_TYPE_MATERIAL, id)
{
}

ComponentMaterial::~ComponentMaterial()
{
}

bool ComponentMaterial::DrawGUI()
{
	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Material stuff...");
	}
	return true;
}