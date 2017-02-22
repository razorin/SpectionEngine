#include "ModuleCamera.h"
#include "SDL/include/SDL.h"
#include "Application.h"
#include "ModuleGUI.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "Parson.h"
#include <math.h>


ModuleCamera::ModuleCamera(const JSON_Object *json) : Module(json)
{
	frustum = Frustum();
	invertHorizontalCamera = (bool)json_object_dotget_boolean(json, "invertHorizontalCamera");
	invertVerticalCamera = (bool)json_object_dotget_boolean(json, "invertVerticalCamera");
}


ModuleCamera::~ModuleCamera()
{
}

bool ModuleCamera::Init()
{
	aspectRatio = App->window->screen_width / App->window->screen_height;
	verticalFov = 60 * DEGTORAD;
	horizontalFov = 60 * DEGTORAD;
	frustum.SetPerspective(horizontalFov, verticalFov);

	SetPosition(math::vec{ 0,0.5f,4 });
	SetLookAt(math::vec{ 0,1,0 }, math::vec{ 0,0,-1 });

	SetPlaneDistances(0.1f, 100.0f);

	frustum.SetKind(FrustumProjectiveSpace::FrustumSpaceGL, FrustumHandedness::FrustumRightHanded);

	return true;
}

bool ModuleCamera::Start()
{
	return true;
}

update_status ModuleCamera::Update(float dt)
{
	mouseBlocked = App->gui->uiInput;

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
	glViewport(0, 0, width, height);
}

void ModuleCamera::SetPlaneDistances(float near, float far)
{
	this->near = near;
	this->far = far;
	frustum.SetViewPlaneDistances(near, far);
}

void ModuleCamera::Move(float dt)
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

	movementSpeed = 2.0f;

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		movement *= 3;


	if (movement.Equals(float3::zero) == false)
	{
		frustum.Translate(movement * movementSpeed * dt / 1000);
		//This line below is required so viewmatrix is actualized.
		SetPosition(frustum.Pos());
	}
}


void ModuleCamera::Zoom(float dt)
{
	float3 movement = float3::zero;
	float zoomSpeed = 10.0f;
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


void ModuleCamera::Rotate(float dt)
{
	Quat rotation = Quat::identity;

	rotationSpeed = 1.0f;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		rotationSpeed *= 3;

	float angleX = 0;
	float angleY = 0;
	float angleChange = rotationSpeed * (dt / 1000);

	fPoint mouseMotion = App->input->GetMouseMotion();
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT && mouseBlocked == false)
	{
		if (invertHorizontalCamera) {
			angleX += (mouseMotion.y / 4) * angleChange;
		}
		else {
			angleX -= (mouseMotion.y / 4) * angleChange;
		}
		if (invertVerticalCamera) {
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