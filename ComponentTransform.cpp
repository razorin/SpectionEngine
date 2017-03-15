#include "ComponentTransform.h"
#include "Globals.h"
#include <assert.h>

//IMGUI Includes
#include "IMGUI\imconfig.h"
#include "IMGUI\imgui.h"
#include "IMGUI\imgui_impl_sdl_gl3.h"
#include "IMGUI\imgui_internal.h"
#include "IMGUI\stb_rect_pack.h"
#include "IMGUI\stb_textedit.h"
#include "IMGUI\stb_truetype.h"


ComponentTransform::ComponentTransform(GameObject* container) : Component(container, ComponentType::COMPONENT_TYPE_TRANSFORM)
{
	ResetTransforms();
}

ComponentTransform::ComponentTransform(GameObject* container, Quat& rotation, float3& position, float3& scale) :
	Component(container, ComponentType::COMPONENT_TYPE_TRANSFORM), rotation(rotation), position(position), scale(scale)
{
	ResetTransforms();
	CalculateTransforms();
}

ComponentTransform::~ComponentTransform()
{
}


void ComponentTransform::ResetTransforms()
{
	localTransform = float4x4::identity;
	globalTransform = float4x4::identity;
}


float3 ComponentTransform::Position(SpaceMode space) const
{
	if (space == SpaceMode::SPACE_LOCAL) {
		return position;
	}
	else {
		return globalTransform.TranslatePart();
	}
}

float3 ComponentTransform::Scale(SpaceMode space) const
{
	if (space == SpaceMode::SPACE_LOCAL) {
		return scale;
	}
	else {
		//return
	}
}

Quat ComponentTransform::Rotation(SpaceMode space) const
{
	if (space == SpaceMode::SPACE_LOCAL) {
		return rotation;
	}
	else {
		//return
	}
}

/**
Returns euler angles in Degrees
*/
float3 ComponentTransform::EulerAngles(SpaceMode space) const
{
	if (space == SpaceMode::SPACE_LOCAL) {
		return eulerAngles * RADTODEG;
	}
	else {
		//return
	}
}

float4x4 ComponentTransform::LocalTransform() const
{
	return localTransform;
}

float4x4 ComponentTransform::GlobalTransform() const
{
	return globalTransform;
}


void ComponentTransform::CalculateTransforms()
{
	CalculateLocalT();
	CalculateGlobalT();
}

void ComponentTransform::CalculateLocalT()
{
	localTransform = float4x4::FromTRS(position, rotation, scale);
}

void ComponentTransform::CalculateGlobalT()
{
	globalTransform = parentGlobalTransform * localTransform;
	container->OnTransformChange();
}



void ComponentTransform::SetPosition(const float3 & position)
{
	this->position = position;
	CalculateTransforms();
}

void ComponentTransform::SetScale(const float3 & scale)
{
	this->scale = scale;
	CalculateTransforms();
}

void ComponentTransform::SetRotation(const Quat & rotation)
{
	this->rotation = rotation;
	this->eulerAngles = rotation.ToEulerXYZ();
	CalculateTransforms();
}

/**
Sets rotation in euler angles
@param eulerAngles the rotation in Degrees
*/
void ComponentTransform::SetEulerAngles(const float3 & eulerAngles)
{
	this->eulerAngles = eulerAngles * DEGTORAD;
	Quat targetRotation = Quat::FromEulerXYZ(this->eulerAngles.x, this->eulerAngles.y, this->eulerAngles.z);
	//this->rotation = Quat::Lerp(rotation, targetRotation, 0.5f);
	this->rotation = targetRotation;
	CalculateTransforms();
}

void ComponentTransform::Rotate(float angle, float3 axis)
{
	Quat changeRotation = Quat(axis, angle);
	rotation = rotation * changeRotation;
	CalculateTransforms();
}

void ComponentTransform::SetTransform(const float3 & position, const float3 & scale, const Quat & rotation)
{
	this->position = position;
	this->scale = scale;
	this->rotation = rotation;
	CalculateTransforms();
}

void ComponentTransform::SetParent(const float4x4 & parentGT)
{
	parentGlobalTransform = parentGT;
	CalculateTransforms();
}

void ComponentTransform::ChangeParent(const float4x4 & newParentGT)
{
	localTransform = newParentGT.Inverted() * (parentGlobalTransform * localTransform);
	localTransform.Decompose(position, rotation, scale);
	globalTransform = newParentGT * localTransform;
}

bool ComponentTransform::DrawGUI() {
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		float3 pos = Position();
		float3 rot = EulerAngles();
		float3 sca = Scale();
		if (ImGui::DragFloat3("Position", (float*)&pos, 0.1f)) {
			SetPosition(pos);
		}
		ImGui::SameLine();
		if (ImGui::Button("Clear Position")) {
			pos = float3::zero;
			SetPosition(pos);
		}

		if (ImGui::DragFloat3("Rotation", (float*)&rot, 0.1f)) {
			SetEulerAngles(rot);
		}
		ImGui::SameLine();
		if (ImGui::Button("Clear Rotation")) {
			rot = float3::zero;
			SetEulerAngles(rot);
		}

		if (ImGui::DragFloat3("Scale", (float*)&sca, 0.1f)) {
			SetScale(sca);
		}
		ImGui::SameLine();
		if (ImGui::Button("Clear Scale")) {
			sca = float3::one;
			SetScale(sca);
		}

		if (ImGui::Button("Clear Transform")) {
			pos = float3::zero;
			SetPosition(pos);
			rot = float3::zero;
			SetEulerAngles(rot);
			sca = float3::one;
			SetScale(sca);
		}
	}
	return true;
}