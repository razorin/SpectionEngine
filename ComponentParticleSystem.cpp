#include "ComponentParticleSystem.h"

//IMGUI Includes
#include "IMGUI\imconfig.h"
#include "IMGUI\imgui.h"
#include "IMGUI\imgui_impl_sdl_gl3.h"
#include "IMGUI\imgui_internal.h"
#include "IMGUI\stb_rect_pack.h"
#include "IMGUI\stb_textedit.h"
#include "IMGUI\stb_truetype.h"



ComponentParticleSystem::ComponentParticleSystem(GameObject * container, std::string id) : Component(container, ComponentType::COMPONENT_TYPE_PARTICLE_SYSTEM, id)
{
	name = "Particle System";
}


ComponentParticleSystem::~ComponentParticleSystem()
{
}

void ComponentParticleSystem::Init(unsigned maxParticles, const aiVector2D & emitArea, unsigned fallingTime, float fallingHeight, const char * textureFile, const aiVector2D particleSize)
{
}

void ComponentParticleSystem::Clear()
{
}

void ComponentParticleSystem::Update(unsigned elapsed, const float3 cameraPosition)
{
}

void ComponentParticleSystem::Draw()
{
}

bool ComponentParticleSystem::DrawGUI()
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
