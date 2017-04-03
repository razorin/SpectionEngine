#include "ComponentBillboarding.h"
#include "ComponentTransform.h"
#include "Glew\include\GL\glew.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleGUI.h"

//IMGUI Includes
#include "IMGUI\imconfig.h"
#include "IMGUI\imgui.h"
#include "IMGUI\imgui_impl_sdl_gl3.h"
#include "IMGUI\imgui_internal.h"
#include "IMGUI\stb_rect_pack.h"
#include "IMGUI\stb_textedit.h"
#include "IMGUI\stb_truetype.h"

ComponentBillboarding::ComponentBillboarding(GameObject* container, std::string id, float2 size, aiString texturePath) : Component(container, ComponentType::COMPONENT_TYPE_BILLBOARDING, id)
{
	name = "Billboard";
	billboard.size = size;
	billboard.texturePath = texturePath;
}


ComponentBillboarding::~ComponentBillboarding()
{
}

void ComponentBillboarding::ComputeQuad(const float3 cameraPosition)
{
	ComponentTransform *transform = (ComponentTransform *)container->FindComponent(ComponentType::COMPONENT_TYPE_TRANSFORM);
	float3 up = float3(0, 1, 0);
	float3 position = transform->Position(SpaceMode::SPACE_GLOBAL);
	float3 directionToCamera = position - cameraPosition;
	float3 right = up.Cross(directionToCamera);

	// Normalize right vector
	if (right.Length() != 0.0f) {
		right = { right.x / right.Length(), right.y / right.Length(), right.z / right.Length() };
	}
	else {
		App->gui->console.AddLog("ERROR ON BILLBOARD: Right vector can't be normalized");
	}

	float3 upperRightCorner = position + (up * billboard.size.y * 0.5 + right * billboard.size.x * 0.5);
	float3 upperLeftCorner = position + (up * billboard.size.y * 0.5 - right * billboard.size.x * 0.5);
	float3 bottomRightCorner = position + (-up * billboard.size.y * 0.5 + right * billboard.size.x * 0.5);
	float3 bottomLeftCorner = position + (-up * billboard.size.y * 0.5 - right * billboard.size.x * 0.5);

	billboard.texture = App->textures->LoadTexture(billboard.texturePath);

	// TODO Change texture printing from direct to buffer mode
	glBindTexture(GL_TEXTURE_2D, billboard.texture);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(bottomLeftCorner.x, bottomLeftCorner.y, bottomLeftCorner.z);
	glTexCoord2f(0, 1);
	glVertex3f(upperLeftCorner.x, upperLeftCorner.y, upperLeftCorner.z);
	glTexCoord2f(1, 1);
	glVertex3f(upperRightCorner.x, upperRightCorner.y, upperRightCorner.z);
	glTexCoord2f(1, 0);
	glVertex3f(bottomRightCorner.x, bottomRightCorner.y, bottomRightCorner.z);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
}

bool ComponentBillboarding::DrawGUI()
{
	std::string headerLabel = name + "##" + id;
	if (ImGui::CollapsingHeader(headerLabel.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		std::string checkboxLabel = "Active##" + id;
		ImGui::Checkbox(checkboxLabel.c_str(), &active);
	}
	std::string removeLabel = "Remove Component##" + id;
	if (ImGui::Button(removeLabel.c_str())) {
		toDelete = true;
	}
	return true;
}