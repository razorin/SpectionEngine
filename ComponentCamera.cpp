#include "ComponentCamera.h"
#include "SDL/include/SDL.h"
#include "ModuleInput.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleGUI.h"

//IMGUI Includes
#include "IMGUI\imconfig.h"
#include "IMGUI\imgui.h"
#include "IMGUI\imgui_impl_sdl_gl3.h"
#include "IMGUI\imgui_internal.h"
#include "IMGUI\stb_rect_pack.h"
#include "IMGUI\stb_textedit.h"
#include "IMGUI\stb_truetype.h"


ComponentCamera::ComponentCamera(GameObject * container, std::string id) : Component(container, ComponentType::COMPONENT_TYPE_CAMERA, id)
{
	aspectRatio = App->window->screen_width / App->window->screen_height;
	verticalFov = 60 * DEGTORAD;
	horizontalFov = 60 * DEGTORAD;
	frustum.SetPerspective(horizontalFov, verticalFov);

	SetPosition(math::vec{ 0,0.5f,4 });
	SetLookAt(math::vec{ 0,1,0 }, math::vec{ 0,0,-1 });

	SetPlaneDistances(0.1f, 100.0f);

	frustum.SetKind(FrustumProjectiveSpace::FrustumSpaceGL, FrustumHandedness::FrustumRightHanded);
}

ComponentCamera::~ComponentCamera()
{
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

void ComponentCamera::Move(float dt)
{
	float3 movement = float3::zero;

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		movement -= frustum.WorldRight();
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		movement += frustum.WorldRight();

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		movement += frustum.Front();
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		movement -= frustum.Front();

	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT)
		movement += float3::unitY;
	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT)
		movement -= float3::unitY;

	movementSpeed = 5.0f;

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		movement *= 3;


	if (movement.Equals(float3::zero) == false)
	{
		frustum.Translate(movement * movementSpeed * dt / 1000);
		//This line below is required so viewmatrix is actualized.
		SetPosition(frustum.Pos());
	}
}


void ComponentCamera::Zoom(float dt)
{
	float3 movement = float3::zero;
	float zoomSpeed = 20.0f;
	int mouseWheel = App->input->GetMouseWheel();

	if (mouseWheel != 0 && mouseBlocked == false) {
		movement += frustum.Front() * mouseWheel * zoomSpeed * dt / 1000;
	}

	if (movement.Equals(float3::zero) == false)
	{
		frustum.Translate(movement);
		//This line below is required so viewmatrix is actualized.
		SetPosition(frustum.Pos());
	}
}


void ComponentCamera::Rotate(float dt)
{
	Quat rotation = Quat::identity;

	rotationSpeed = 2.0f;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		rotationSpeed *= 3;

	float angleX = 0;
	float angleY = 0;
	float angleChange = rotationSpeed * (dt / 1000);

	fPoint mouseMotion = App->input->GetMouseMotion();
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT && mouseBlocked == false)
	{
		if (invertYAxis) {
			angleX += (mouseMotion.y / 4) * angleChange;
		}
		else {
			angleX -= (mouseMotion.y / 4) * angleChange;
		}
		if (invertXAxis) {
			angleY += (mouseMotion.x / 4) * angleChange;
		}
		else {
			angleY -= (mouseMotion.x / 4) * angleChange;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		angleY += angleChange;
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		angleY -= angleChange;
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		angleX += angleChange;
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		angleX -= angleChange;

	if (angleY != 0)
	{
		rotation = Quat::RotateY(angleY);
		frustum.SetFront(rotation.Transform(frustum.Front()));
		frustum.SetUp(rotation.Transform(frustum.Up()));
	}
	if (angleX != 0)
	{
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

// TODO Instead of a float it will receive a rotation matrix that will be multiplied by frustum->Up and frustum->Front
void ComponentCamera::SetOrientation(Axis axis, float rotation)
{
	switch (axis) {
	case X:
		break;
	case Y:
		break;
	case Z:
		break;
	}
}

// TODO An assert must check Front and Up vectors are in a 90 degree angle
void ComponentCamera::SetLookAt(const math::vec & up, const math::vec & front)
{
	frustum.SetUp(up);
	frustum.SetFront(front);

	//frustum.GetCornerPoints()
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
	Move(dt);
	Zoom(dt);
	Rotate(dt);

	// Print position and orientation
	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) {
		App->gui->console.AddLog("Camera position: %f, %f, %f", pos.x, pos.y, pos.z);
		DLOG("Camera position: %f, %f, %f", pos.x, pos.y, pos.z);
	}
	if (App->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN) {
		App->gui->console.AddLog("Camera UP vector: %f, %f, %f", frustum.Up().x, frustum.Up().y, frustum.Up().z);
		DLOG("Camera UP vector: %f, %f, %f", frustum.Up().x, frustum.Up().y, frustum.Up().z);
		App->gui->console.AddLog("Camera FRONT vector: %f, %f, %f", frustum.Front().x, frustum.Front().y, frustum.Front().z);
		DLOG("Camera FRONT vector: %f, %f, %f", frustum.Front().x, frustum.Front().y, frustum.Front().z);
	}
}

bool ComponentCamera::DrawGUI()
{
	if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
	{
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
	return true;
}

void ComponentCamera::setMouseBlocked(bool mouseBlocked)
{
	this->mouseBlocked = mouseBlocked;
}

bool ComponentCamera::ContainsAaBox(const math::AABB& refBox) const
{
	math::vec vCorner[8];
	int iTotalIn = 0;
	refBox.GetCornerPoints(vCorner);
	math::Plane* planes;
	refBox.GetFacePlanes(planes);
	for (int p = 0; p < 6; ++p) {
		int iInCount = 8;
		int iPtIn = 1;
		for (int i = 0; i < 8; ++i) {
			if (planes[p].IsOnPositiveSide(vCorner[i]) == false) {
				iPtIn = 0;
				--iInCount;
			}
		}
		if (iInCount == 0) {
			return false;
		}
		iTotalIn += iPtIn;
	}
	if (iTotalIn == 6) {
		return true;
	}
	return true;
}