#include "Billboard.h"
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


Billboard::Billboard()
{
	size = { 1,1 };
}

Billboard::Billboard(float2 size) : size(size)
{
}

Billboard::~Billboard()
{
}

std::vector<float3> Billboard::ComputeQuad(const float3 cameraPosition, aiVector3D particlePosition)
{
	float3 up = float3(0, 1, 0);
	float3 position = { particlePosition.x,particlePosition.y,particlePosition.z };
	float3 directionToCamera = position - cameraPosition;
	float3 right = up.Cross(directionToCamera);

	// Normalize right vector
	if (right.Length() != 0.0f) {
		right = { right.x / right.Length(), right.y / right.Length(), right.z / right.Length() };
	}
	else {
		App->gui->console.AddLog("ERROR ON BILLBOARD: Right vector can't be normalized");
	}

	float3 upperRightCorner = position + (up * size.y * 0.5 + right * size.x * 0.5);
	float3 upperLeftCorner = position + (up * size.y * 0.5 - right * size.x * 0.5);
	float3 bottomRightCorner = position + (-up * size.y * 0.5 + right * size.x * 0.5);
	float3 bottomLeftCorner = position + (-up * size.y * 0.5 - right * size.x * 0.5);
	
	std::vector<float3> ret = { upperRightCorner,upperLeftCorner,bottomRightCorner,bottomLeftCorner };
	return ret;
}

void Billboard::SetSize(float2 value) {
	this->size = value;
}