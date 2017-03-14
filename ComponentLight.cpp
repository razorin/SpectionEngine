#include "ComponentLight.h"

//IMGUI Includes
#include "IMGUI\imconfig.h"
#include "IMGUI\imgui.h"
#include "IMGUI\imgui_impl_sdl_gl3.h"
#include "IMGUI\imgui_internal.h"
#include "IMGUI\stb_rect_pack.h"
#include "IMGUI\stb_textedit.h"
#include "IMGUI\stb_truetype.h"



ComponentLight::ComponentLight(GameObject * container) : Component(container, ComponentType::COMPONENT_TYPE_LIGHT)
{
}

ComponentLight::~ComponentLight()
{
}

bool ComponentLight::DrawGUI()
{
	if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Light stuff...");
	}
	return true;
}