#include "ComponentBillboarding.h"
#include "ComponentTransform.h"
#include "Glew\include\GL\glew.h"
#include "Application.h"
#include "ModuleTextures.h"

ComponentBillboarding::ComponentBillboarding(GameObject* container, std::string id, float2 size, aiString texturePath) : Component(container, ComponentType::COMPONENT_TYPE_BILLBOARDING,id),
	size(size), texturePath(texturePath)
{
}


ComponentBillboarding::~ComponentBillboarding()
{
}

void ComponentBillboarding::ComputeQuad(float3 cameraPosition)
{
	ComponentTransform *transform = (ComponentTransform *)container->FindComponent(ComponentType::COMPONENT_TYPE_BILLBOARDING);
	float3 up = float3(0, 1, 0);
	float3 position = transform->Position(SpaceMode::SPACE_GLOBAL);
	float3 directionToCamera = position - cameraPosition;
	float3 right = up.Cross(directionToCamera);

	float3 upperRightCorner = position + (up * size.y * 0.5 + right * size.x * 0.5);
	float3 upperLeftCorner = position + (up * size.y * 0.5 - right * size.x * 0.5);
	float3 bottomRightCorner = position + ( -up * size.y * 0.5 + right * size.x * 0.5);
	float3 bottomLeftCorner = position + (-up * size.y * 0.5 - right * size.x * 0.5);

	glBegin(GL_TRIANGLES);
	glVertex3f(upperRightCorner.x, upperRightCorner.y, upperRightCorner.z);
	glVertex3f(upperLeftCorner.x, upperLeftCorner.y, upperLeftCorner.z);
	glVertex3f(bottomRightCorner.x, bottomRightCorner.y, bottomRightCorner.z);
	glVertex3f(upperLeftCorner.x, upperLeftCorner.y, upperLeftCorner.z);
	glVertex3f(bottomLeftCorner.x, bottomLeftCorner.y, bottomLeftCorner.z);
	glVertex3f(bottomRightCorner.x, bottomRightCorner.y, bottomRightCorner.z);
	glEnd();

	texture = App->textures->LoadTexture(texturePath);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT,
	//	0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
	/*glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT,
		0, GL_RGBA, GL_UNSIGNED_BYTE, &textureHandle);
		*/
	glBindTexture(GL_TEXTURE_2D, 0);

}

bool ComponentBillboarding::DrawGUI()
{
	return true;
}


