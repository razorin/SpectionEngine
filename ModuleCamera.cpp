#include "ModuleCamera.h"
#include "SDL/include/SDL.h"
#include "Application.h"
#include "ModuleWindow.h"
#include <math.h>
#include "Application.h"
#include "ModuleInput.h"


ModuleCamera::ModuleCamera(const JSON_Object *json) : Module(json)
{
	frustum = Frustum();
}


ModuleCamera::~ModuleCamera()
{
}

bool ModuleCamera::Init()
{
	aspectRatio = App->window->screen_width / App->window->screen_height;
	verticalFov = 30 * DEGTORAD;
	horizontalFov = 30 * DEGTORAD;
	frustum.SetPerspective(horizontalFov, verticalFov);

	SetPosition(math::vec{ 5,0,0 });
	SetLookAt(math::vec{ 0,1,0 }, math::vec{ -1,0,0 });

	SetPlaneDistances(0.1f, 100.0f);

	frustum.SetKind(FrustumProjectiveSpace::FrustumSpaceGL, FrustumHandedness::FrustumRightHanded);

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
	// Print position and orientation
	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) {
		DLOG("Camera position: %f, %f, %f", pos.x, pos.y, pos.z);
	}
	if (App->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN) {
		DLOG("Camera UP vector: %f, %f, %f", frustum.Up().x, frustum.Up().y, frustum.Up().z);
		DLOG("Camera FRONT vector: %f, %f, %f", frustum.Front().x, frustum.Front().y, frustum.Front().z);
	}

	double speed = 0.01;

	// We asume X axis -> Pitch, Y axis -> Yaw, Z axis -> Roll
	// Camera pitch
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) {
		DLOG("Pressing UP arrow");
		RotateCamera(X, floor(speed*dt));
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
		DLOG("Pressing DOWN arrow");
		RotateCamera(X, -floor(speed*dt));
	}
	
	// Camera yaw
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
		DLOG("Pressing RIGHT arrow");
		RotateCamera(X, floor(speed*dt));
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
		DLOG("Pressing LEFT arrow");
		RotateCamera(X, -floor(speed*dt));
	}
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
	frustum.SetVerticalFovAndAspectRatio(verticalFov, this->aspectRatio);
}

void ModuleCamera::SetAspectRatio(float aspectRatio)
{
	this->aspectRatio = aspectRatio;
	horizontalFov = 2 * atan(tan(verticalFov * 0.5) * aspectRatio);
	frustum.SetHorizontalFovAndAspectRatio(this->horizontalFov, aspectRatio);
}

void ModuleCamera::ChangeWindowSize(int width, int height)
{
	App->window->screen_width = width;
	App->window->screen_height = height;
	aspectRatio = (float)width / (float)height;
	SetAspectRatio(aspectRatio);

	//verticalFov = 2 * atan(tan(horizontalFov * 0.5) * 1 / aspectRatio);
	//horizontalFov = 2 * atan(tan(verticalFov * 0.5) * aspectRatio);
}

void ModuleCamera::SetPlaneDistances(float near, float far)
{
	this->near = near;
	this->far = far;
	frustum.SetViewPlaneDistances(near, far);
}

float * ModuleCamera::GetMatrixProjection() const
{
	float4x4 projectionMatrix = frustum.ProjectionMatrix();
	float4x4 m = projectionMatrix.Transposed();
	return &(m[0][0]);
}

float * ModuleCamera::GetMatrixView() const
{
	float4x4 viewMatrix = frustum.ViewMatrix();
	float4x4 m = viewMatrix.Transposed();
	return &(m[0][0]);
}
void ModuleCamera::SetPosition(const math::vec &pos)
{
	this->pos = pos;
	frustum.SetPos(pos);
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
	frustum.SetUp(up);
	frustum.SetFront(front);
}

void ModuleCamera::RotateCamera(Axis axis, float rotation)
{
	Quat quat;
	math::vec u;
	float s;
	math::vec vprime;

	switch (axis) {
	case X:
		DLOG("Rotating camera on X axis");
		quat = { frustum.WorldRight(), rotation };

		// Extract the vector part of the quaternion
		u = { quat.x, quat.y, quat.z };

		// Extract the scalar part of the quaternion
		s = quat.w;

		// Do the math
		vprime = 2.0f * math::Dot(u, frustum.WorldRight()) * u
			+ (s*s - math::Dot(u, u)) * frustum.WorldRight()
			+ 2.0f * s * math::Cross(u, frustum.WorldRight());
		
		frustum.SetFront(vprime);
		break;
	case Y:
		break;
	case Z:
		break;
	}
}
