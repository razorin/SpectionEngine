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

	SetPosition(math::vec{ 7,1,0 });
	SetLookAt(math::vec{ 0,1,0 }, math::vec{ -1,0,0 });

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

	Move(dt);
	Zoom(dt);
	Rotate(dt);


	// Print position and orientation
	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) {
		DLOG("Camera position: %f, %f, %f", pos.x, pos.y, pos.z);
	}
	if (App->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN) {
		DLOG("Camera UP vector: %f, %f, %f", frustum.Up().x, frustum.Up().y, frustum.Up().z);
		DLOG("Camera FRONT vector: %f, %f, %f", frustum.Front().x, frustum.Front().y, frustum.Front().z);
	}

	//float speed = 0.01f;

	//// We asume X axis -> Pitch, Y axis -> Yaw, Z axis -> Roll
	//// Camera pitch
	//if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN) {
	//	RotateCamera(X, speed*dt);
	//}

	//if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN) {
	//	RotateCamera(X, -floor(speed*dt));
	//}
	//
	//// Camera yaw
	//if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN) {
	//	RotateCamera(Y, floor(speed*dt));
	//}

	//if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN) {
	//	RotateCamera(Y, -floor(speed*dt));
	//}
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
		movement *= 2;


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
	
	if (mouseWheel != 0) 
		movement += frustum.Front() * mouseWheel * zoomSpeed * dt/1000;

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

	rotationSpeed = 0.5f;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		rotationSpeed *= 2;

	float angleX = 0;
	float angleY = 0;
	float angleChange = rotationSpeed * (dt / 1000);

	fPoint mouseMotion = App->input->GetMouseMotion();
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
	{
		angleX += (mouseMotion.y / 2) * angleChange;
		angleY += (mouseMotion.x / 2) * angleChange;
		DLOG("MousemotionY %f", mouseMotion.y);
		DLOG("MousemotionX %f", mouseMotion.x);

	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		angleY += angleChange;
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		angleY -= angleChange;
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		angleX -= angleChange;
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		angleX += angleChange;

	if (angleY != 0)
		rotation = Quat::RotateY(angleY);
	if (angleX != 0)
		rotation = Quat::RotateAxisAngle(frustum.WorldRight(), (angleX));

	if (rotation.Equals(Quat::identity) == false)
	{
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

void ModuleCamera::RotateCamera(Axis axis, float rotation)
{
	Quat quat;
	math::vec u;
	float s;
	math::vec vprime;

	switch (axis) {
	case X:
		DLOG("Rotating camera on X axis %f", rotation);
		
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