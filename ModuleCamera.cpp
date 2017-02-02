#include "ModuleCamera.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include <math.h>
#include "Application.h"
#include "ModuleInput.h"



ModuleCamera::ModuleCamera(const JSON_Object *json) : Module(json)
{
	frustum = new Frustum();
	verticalFov = 2 * atan(tan(horizontalFov * 0.5) * 1 / aspectRatio);
	horizontalFov = 2 * atan(tan(verticalFov * 0.5) * aspectRatio);
}


ModuleCamera::~ModuleCamera()
{
}

bool ModuleCamera::Init()
{
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
	//TODO: Change code location
	projectionMatrix = frustum->ViewProjMatrix();
	projectionMatrix.Transpose();

	viewMatrix = frustum->ViewMatrix();
	viewMatrix.Transpose3();

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

void ModuleCamera::SetPlaneDistances(float near, float far)
{
	this->near = near;
	this->far = far;
	frustum->SetViewPlaneDistances(near, far);
}

float4x4 ModuleCamera::GetMatrixProjection() const
{
	return projectionMatrix;
}

float3x4 ModuleCamera::GetMatrixView() const
{
	return viewMatrix;
}
