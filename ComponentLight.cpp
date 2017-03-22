#include "ComponentLight.h"
#include "Application.h"
#include "LightsManager.h"
#include "Light.h"

//IMGUI Includes
#include "IMGUI\imconfig.h"
#include "IMGUI\imgui.h"
#include "IMGUI\imgui_impl_sdl_gl3.h"
#include "IMGUI\imgui_internal.h"
#include "IMGUI\stb_rect_pack.h"
#include "IMGUI\stb_textedit.h"
#include "IMGUI\stb_truetype.h"



ComponentLight::ComponentLight(GameObject * container, std::string id, Light* light) : Component(container, ComponentType::COMPONENT_TYPE_LIGHT, id), light(light)
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
		const char* items[] = { "DIRECTIONAL LIGHT", "POINT LIGHT", "SPOTLIGHT", "AMBIENTLIGHT" };

		std::string checkboxLabel = "Active##" + id;
		if (ImGui::Checkbox(checkboxLabel.c_str(), &active)) {
			if (active) {
				App->lightsManager->EnableLight(light);
			}
			else {
				App->lightsManager->DisableLight(light);
			}
		}

		int newType = light->type;
		// Generic strings
		std::string typeName = "Type##" + id;
		std::string position = "Position##" + id;
		std::string diffuse = "Diffuse##" + id;
		std::string ambient = "Ambient##" + id;
		std::string specular = "Specular##" + id;
		std::string direction = "Direction##" + id;
		// Extra strings
		std::string exponent = "Exponent##" + id;
		std::string cutoff = "Cutoff##" + id;
		std::string constantAttenuation = "Constant Attenuation##" + id;
		std::string linearAttenuation = "Linear Attenuation##" + id;
		std::string quadraticAttenuation = "Quadratic Attenuation##" + id;
		if (ImGui::Combo(typeName.c_str(), &newType, items, IM_ARRAYSIZE(items))) {
			light->type = static_cast<LightType>(newType);
		}
		if (light->type == LT_DIRECTIONAL_LIGHT) {
			ImGui::DragFloat3(direction.c_str(), light->position, 0.1f);
		}
		else {
			ImGui::DragFloat3(position.c_str(), light->position, 0.1f);
		}
		ImGui::ColorEdit3(diffuse.c_str(), light->diffuse);
		ImGui::ColorEdit3(ambient.c_str(), light->ambient);
		ImGui::ColorEdit3(specular.c_str(), light->specular);
		if (light->type == LT_SPOTLIGHT_LIGHT) {
			ImGui::DragFloat3(direction.c_str(), light->direction, 0.1f);
			ImGui::DragFloat(exponent.c_str(), &light->exponent, 0.1f);
			ImGui::DragFloat(cutoff.c_str(), &light->cutoff, 0.1f);
		}
		if (ImGui::TreeNode("More options"))
		{
			ImGui::DragFloat(constantAttenuation.c_str(), &light->constantAttenuation, 0.1f);
			ImGui::DragFloat(linearAttenuation.c_str(), &light->linearAttenuation, 0.1f);
			ImGui::DragFloat(quadraticAttenuation.c_str(), &light->quadraticAttenuation, 0.1f);
			ImGui::TreePop();
		}
	}
	std::string removeLabel = "Remove Component##" + id;
	if (ImGui::Button(removeLabel.c_str())) {
		App->lightsManager->DisableLight(light);
		toDelete = true;
	}
	return true;
}