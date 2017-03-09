#include "ComponentCamera.h"
#include "SDL/include/SDL.h"
#include "ModuleInput.h"
#include "Application.h"
#include "ModuleWindow.h"


ComponentCamera::ComponentCamera(GameObject * container) : Component(container, ComponentType::COMPONENT_TYPE_CAMERA)
{
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