#include "ComponentCamera.h"
#include "SDL/include/SDL.h"
#include "ModuleInput.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleGUI.h"
#include <vector>

//IMGUI Includes
#include "IMGUI\imconfig.h"
#include "IMGUI\imgui.h"
#include "IMGUI\imgui_impl_sdl_gl3.h"
#include "IMGUI\imgui_internal.h"
#include "IMGUI\stb_rect_pack.h"
#include "IMGUI\stb_textedit.h"
#include "IMGUI\stb_truetype.h"

#include "ModuleCamera.h"

ComponentCamera::ComponentCamera(GameObject * container, std::string id) : Component(container, ComponentType::COMPONENT_TYPE_CAMERA, id)
{
	name = "Camera";
	aspectRatio = App->window->screen_width / (float)App->window->screen_height;

	verticalFov = 60 * DEGTORAD;
	horizontalFov = 60 * DEGTORAD;

	SetAspectRatio(aspectRatio);
	
	frustum.SetPerspective(horizontalFov, verticalFov);

	SetPosition(math::vec{ 0,2,4 });
	SetLookAt(math::vec{ 0,1,0 }, math::vec{ 0,0,-1 });

	SetPlaneDistances(0.1f, 300.0f);

	frustum.SetKind(FrustumProjectiveSpace::FrustumSpaceGL, FrustumHandedness::FrustumRightHanded);

	App->camera->AddCamera(this);
}

ComponentCamera::~ComponentCamera()
{
	App->camera->RemoveCamera(this);
}

void ComponentCamera::SetFOV(float verticalFov)
{
	this->verticalFov = verticalFov;
	frustum.SetVerticalFovAndAspectRatio(verticalFov, this->aspectRatio);
}

void ComponentCamera::SetAspectRatio(float aspectRatio)
{
	this->aspectRatio = aspectRatio;
	horizontalFov = 2 * atan(tan(verticalFov * 0.5) * aspectRatio);
	frustum.SetHorizontalFovAndAspectRatio(this->horizontalFov, aspectRatio);
}

void ComponentCamera::ChangeWindowSize(int width, int height)
{
	App->window->screen_width = width;
	App->window->screen_height = height;
	aspectRatio = (float)width / (float)height;
	SetAspectRatio(aspectRatio);
	glViewport(0, 0, width, height);
}

void ComponentCamera::SetPlaneDistances(float near, float far)
{
	this->nearPlane = near;
	this->farPlane = far;
	frustum.SetViewPlaneDistances(near, far);
}

void ComponentCamera::Traslate(const float3 & movement, const float angleX, const float angleY) {
	Quat rotation = Quat::identity;

	if (movement.Equals(float3::zero) == false) {
		frustum.Translate(movement);
		//This line below is required so viewmatrix is actualized.
		SetPosition(frustum.Pos());
	}

	if (angleY != 0) {
		rotation = Quat::RotateY(angleY);
		frustum.SetFront(rotation.Transform(frustum.Front()));
		frustum.SetUp(rotation.Transform(frustum.Up()));
	}
	if (angleX != 0) {
		rotation = Quat::RotateAxisAngle(frustum.WorldRight(), (angleX));
		frustum.SetFront(rotation.Transform(frustum.Front()));
		frustum.SetUp(rotation.Transform(frustum.Up()));
	}
}

void ComponentCamera::SetPosition(const math::vec &pos)
{
	this->pos = pos;
	frustum.SetPos(pos);
}

// Sets the camera Up vector and Front vector
void ComponentCamera::SetLookAt(const math::vec & up, const math::vec & front)
{
	frustum.SetUp(up);
	frustum.SetFront(front);
}

float * ComponentCamera::GetMatrixProjection() const
{
	float4x4 projectionMatrix = frustum.ProjectionMatrix();
	float4x4 m = projectionMatrix.Transposed();
	return &(m[0][0]);
}

float * ComponentCamera::GetMatrixView() const
{
	float4x4 viewMatrix = frustum.ViewMatrix();
	float4x4 m = viewMatrix.Transposed();
	return &(m[0][0]);
}

void ComponentCamera::Update(float dt)
{
	
}

void ComponentCamera::SetMouseBlocked(bool mouseBlocked)
{
	this->mouseBlocked = mouseBlocked;
}

// TODO: Optimize code
void ComponentCamera::DrawFrustum() {
	// Frustum CornerPoint order
	//    2-------6	
	//   /|      /| 
	//  3-------7 |	
	//  | |     | |	
	//  | |0----|-|4
	//  |/      |/  
	//  1-------5  	
	math::vec corners[8];
	for (int i = 0; i < 8; ++i) {
		corners[i] = frustum.CornerPoint(i);
	}
	int vertexOrder[24] = { 0,1,1,3,3,2,2,0,0,4,1,5,3,7,2,6,4,5,5,7,7,6,6,4 };
	glBegin(GL_LINES);
	for (int i = 0; i < 24; ++i) {
		glVertex3f(corners[vertexOrder[i]].x, corners[vertexOrder[i]].y, corners[vertexOrder[i]].z);
	}
	glEnd();
}

bool ComponentCamera::ContainsAaBox(const math::AABB& refBox) const
{
	bool ret = true;
	math::vec vCorner[8];
	int iTotalIn = 0;
	refBox.GetCornerPoints(vCorner);
	math::Plane planes[6];
	frustum.GetPlanes(planes);
	for (int p = 0; p < 6; ++p) {
		int iInCount = 8;
		int iPtIn = 1;
		for (int i = 0; i < 8; ++i) {
			if (planes[p].IsOnPositiveSide(vCorner[i]) == true) {
				iPtIn = 0;
				--iInCount;
			}
		}
		if (iInCount == 0) {
			ret = false;
			break;
		}
		iTotalIn += iPtIn;
	}
	return ret;
}

bool ComponentCamera::DrawGUI()
{
	std::string headerLabel = name + "##" + id;
	if (ImGui::CollapsingHeader(headerLabel.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		std::string checkboxLabel2 = "Frustum Culling##" + id;
		ImGui::Checkbox(checkboxLabel2.c_str(), &frustumCulling);

		std::string checkboxLabelActive = "Active Camera##" + id;
		if (ImGui::Checkbox(checkboxLabelActive.c_str(), &active)) {
			App->camera->ChangeActiveCamera(this);
		}

		float fov = this->verticalFov;
		float near = this->nearPlane;
		float far = this->farPlane;

		if (ImGui::DragFloat("Vertical FOV", (float*)&fov, 0.1f)) {
			SetFOV(fov);
		}

		if (ImGui::DragFloat("Near Plane", (float*)&near, 0.1f)) {
			SetPlaneDistances(near, far);
		}

		if (ImGui::DragFloat("Far Plane", (float*)&far, 0.1f)) {
			SetPlaneDistances(near, far);
		}
	}
	std::string removeLabel = "Remove Component##" + id;
	if (ImGui::Button(removeLabel.c_str())) {
		toDelete = true;
	}
	return true;
}

bool ComponentCamera::IsFrustumCulling()
{
	return frustumCulling;
}
