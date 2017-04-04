#include "ComponentParticleSystem.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "Billboard.h"
#include <stdlib.h>
#include <time.h>

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
	RELEASE(vertices);
	RELEASE(textureCoords);
	RELEASE(colors);
	RELEASE(indices);
}

void ComponentParticleSystem::Init(unsigned maxParticles, const aiVector2D & emitArea, unsigned fallingTime,
	float fallingHeight, const char * textureFile, const float2 particleSize)
{
	this->maxParticles = maxParticles;
	this->emitArea = emitArea;
	this->fallingTime = fallingTime;
	this->fallingHeight = fallingHeight;
	this->texture = App->textures->LoadTexture(aiString(textureFile));
	this->particleSize = particleSize;

	aiVector3D particleVelocity = { 0, fallingHeight / fallingTime, 0 };

	int numSideParticles = trunc(sqrt(maxParticles));

	aiVector2D spacing;
	spacing.x = emitArea.x / numSideParticles;
	spacing.y = emitArea.y / numSideParticles;
	aiVector2D particlePos = { -emitArea.x / 2, -emitArea.y / 2 };

	srand(time(NULL));

	for (int i = 0; i < numSideParticles; ++i) {
		for (int j = 0; j < numSideParticles; ++j) {
			float yInitPos = (float)(rand() % ((int)fallingHeight*1000))/1000;
			Particle tempPart = Particle({ particlePos.x,yInitPos,particlePos.y }, particleVelocity, fallingTime, Billboard(particleSize));
			particles.push_back(tempPart);

			particlePos.y += spacing.y;
		}
		particlePos.y = -emitArea.y / 2;
		particlePos.x += spacing.x;
	}
}

void ComponentParticleSystem::Clear()
{
}

void ComponentParticleSystem::Update(const float3 cameraPosition)
{
	for (ParticlePool::iterator it = particles.begin(); it != particles.end(); ++it) {
		(*it).position -= (*it).velocity;
		if ((*it).position.y < 0) {
			(*it).position.y = fallingHeight;
		}
		(*it).quad = (*it).billboard.ComputeQuad(cameraPosition, (*it).position);
	}
}

void ComponentParticleSystem::Draw()
{
	for (ParticlePool::iterator it = particles.begin(); it != particles.end(); ++it) {

		float3 upperRightCorner, upperLeftCorner, bottomRightCorner, bottomLeftCorner;
		upperRightCorner = (*it).quad[0];
		upperLeftCorner = (*it).quad[1];
		bottomRightCorner = (*it).quad[2];
		bottomLeftCorner = (*it).quad[3];

		DLOG("PARTICLE POS: %f, %f, %f", (*it).position.x, (*it).position.y, (*it).position.z);

		// TODO Change texture printing from direct to buffer mode
		glBindTexture(GL_TEXTURE_2D, texture);
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
}

bool ComponentParticleSystem::DrawGUI()
{
	const char* items[] = { "Rain", "Snow", "Grass" };
	int selection = 0;
	const char* texturePaths[3] = { "Models/Particles/rainSprite.tga","Models/Particles/simpleflake.tga","Models/Grass/billboardgrass.png" };

	unsigned newMaxParticles = maxParticles;
	aiVector2D newEmitArea = emitArea;
	unsigned newFallingTime = fallingTime;
	float newFallingHeight = fallingHeight;
	const char * newTextureFile;
	float2 newParticleSize = particleSize;

	std::string headerLabel = name + "##" + id;
	if (ImGui::CollapsingHeader(headerLabel.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		std::string checkboxLabel = "Active##" + id;
		ImGui::Checkbox(checkboxLabel.c_str(), &active);

		std::string comboLabel = "##" + id;
		if (ImGui::Combo(comboLabel.c_str(), &selection, items, IM_ARRAYSIZE(items))) {
			newTextureFile = texturePaths[selection];
		}
		//Init(newMaxParticles, newEmitArea, newFallingTime, newFallingHeight, newTextureFile, newParticleSize);
	}
	std::string removeLabel = "Remove Component##" + id;
	if (ImGui::Button(removeLabel.c_str())) {
		toDelete = true;
	}
	return true;
}
