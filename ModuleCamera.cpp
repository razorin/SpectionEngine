#include "ModuleCamera.h"
#include "SDL/include/SDL.h"
#include "Application.h"
#include "ModuleWindow.h"
#include <math.h>
#include "Application.h"
#include "ModuleInput.h"


ModuleCamera::ModuleCamera(const JSON_Object *json) : Module(json)
{
	frustum = new Frustum();
}


ModuleCamera::~ModuleCamera()
{
}

bool ModuleCamera::Init()
{
	aspectRatio = App->window->screen_width / App->window->screen_height;
	verticalFov = 2 * atan(tan(horizontalFov * 0.5) * 1 / aspectRatio);
	horizontalFov = 2 * atan(tan(verticalFov * 0.5) * aspectRatio);
	return true;
}

bool ModuleCamera::Start()
{
	return true;
}

update_status ModuleCamera::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleCamera::Update(float dt)
{

	return UPDATE_CONTINUE;
}

update_status ModuleCamera::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleCamera::CleanUp()
{
	return true;
}

void ModuleCamera::SetFOV(float verticalFov)
{
	this->verticalFov = verticalFov;
	frustum->SetVerticalFovAndAspectRatio(verticalFov, this->aspectRatio);
}

void ModuleCamera::SetAspectRatio(float aspectRatio)
{
	this->aspectRatio = aspectRatio;
	horizontalFov = 2 * atan(tan(verticalFov * 0.5) * aspectRatio);
	frustum->SetHorizontalFovAndAspectRatio(this->horizontalFov, aspectRatio);
}

void ModuleCamera::ChangeWindowSize(int width, int height)
{
	App->window->screen_width = width;
	App->window->screen_height = height;
	aspectRatio = width / height;
	verticalFov = 2 * atan(tan(horizontalFov * 0.5) * 1 / aspectRatio);
	horizontalFov = 2 * atan(tan(verticalFov * 0.5) * aspectRatio);
}

void ModuleCamera::SetPlaneDistances(float near, float far)
{
	this->near = near;
	this->far = far;
	frustum->SetViewPlaneDistances(near, far);
}

float4x4 ModuleCamera::GetMatrixProjection() const
{
	float4x4 projectionMatrix = frustum->ViewProjMatrix();
	projectionMatrix.Transpose();
	return projectionMatrix;
}

float3x4 ModuleCamera::GetMatrixView() const
{
	float3x4 viewMatrix = frustum->ViewMatrix();
	viewMatrix.Transpose3();
	return viewMatrix;
}
void ModuleCamera::SetPosition(const math::vec &pos)
{
	this->pos = pos;
	frustum->SetPos(pos);
}

// TODO Instead of a float it will receive a rotation matrix that will be multiplied by frustum->Up and frustum->Front
void ModuleCamera::SetOrientation(Axis axis, float rotation)
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
void ModuleCamera::SetLookAt(const math::vec & up, const math::vec & front)
{
	frustum->SetUp(up);
	frustum->SetFront(front);
}
