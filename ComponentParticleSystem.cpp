#include "ComponentParticleSystem.h"
#include "Application.h"
#include "ModuleTextures.h"

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

void ComponentParticleSystem::Init(unsigned maxParticles, const aiVector2D & emitArea, unsigned fallingTime,
	float fallingHeight, const char * textureFile, const float2 particleSize)
{
	// TODO: Call init on creating the component after its constructor
	// Call: Init(20, { 5,5 }, 2, 4, "Models/Grass/billboardgrass.png", { 1,1 });
	texture = App->textures->LoadTexture(aiString(textureFile));
	this->emitArea = emitArea;
	this->fallingTime = fallingTime;
	this->fallingHeight = fallingHeight;

	aiVector2D spacing;
	spacing.x = emitArea.x / maxParticles;
	spacing.y = emitArea.y / maxParticles;
	aiVector2D particlePos = { -emitArea.x, -emitArea.y };

	for (int i = 0; i < maxParticles; ++i) {
		Billboard tempBill = Billboard(particleSize);
		Particle tempPart = Particle({ particlePos.x,0,particlePos.y }, { 0,0,0 }, 0, tempBill);
		particles.push_back(tempPart);

		particlePos.x += spacing.x;
		particlePos.y += spacing.y;
	}
}

void ComponentParticleSystem::Clear()
{
}

void ComponentParticleSystem::Update(unsigned elapsed, const float3 cameraPosition)
{
}

void ComponentParticleSystem::Draw()
{
	// TODO: For each particle print texture getting its billboard corners
	//// TODO Change texture printing from direct to buffer mode
	//glBindTexture(GL_TEXTURE_2D, billboard.texture);
	//glBegin(GL_QUADS);
	//glTexCoord2f(0, 0);
	//glVertex3f(bottomLeftCorner.x, bottomLeftCorner.y, bottomLeftCorner.z);
	//glTexCoord2f(0, 1);
	//glVertex3f(upperLeftCorner.x, upperLeftCorner.y, upperLeftCorner.z);
	//glTexCoord2f(1, 1);
	//glVertex3f(upperRightCorner.x, upperRightCorner.y, upperRightCorner.z);
	//glTexCoord2f(1, 0);
	//glVertex3f(bottomRightCorner.x, bottomRightCorner.y, bottomRightCorner.z);
	//glEnd();
	//glBindTexture(GL_TEXTURE_2D, 0);
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
