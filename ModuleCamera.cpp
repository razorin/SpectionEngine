#include "ModuleCamera.h"
#include "SDL/include/SDL.h"
#include "Application.h"
#include "ModuleGUI.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "ComponentCamera.h"
#include "Parson.h"
#include <math.h>


ModuleCamera::ModuleCamera(const JSON_Object *json) : Module(json)
{
	frustum = Frustum();
	invertXAxis = (bool)json_object_dotget_boolean(json, "invertHorizontalCamera");
	invertYAxis = (bool)json_object_dotget_boolean(json, "invertVerticalCamera");
}


ModuleCamera::~ModuleCamera()
{
	RELEASE(activeCamera);
}

bool ModuleCamera::Init()
{

	activeCamera = new ComponentCamera(nullptr, "");
	//aspectRatio = App->window->screen_width / App->window->screen_height;
	//verticalFov = 60 * DEGTORAD;
	//horizontalFov = 60 * DEGTORAD;
	//frustum.SetPerspective(horizontalFov, verticalFov);

	//SetPosition(math::vec{ 0,0.5f,4 });
	//SetLookAt(math::vec{ 0,1,0 }, math::vec{ 0,0,-1 });

	//SetPlaneDistances(0.1f, 100.0f);

	//frustum.SetKind(FrustumProjectiveSpace::FrustumSpaceGL, FrustumHandedness::FrustumRightHanded);

	return true;
}

bool ModuleCamera::Start()
{
	return true;
}

update_status ModuleCamera::Update(float dt)
{
	mouseBlocked = App->gui->uiInput;
	
	activeCamera->SetMouseBlocked(mouseBlocked);
	activeCamera->Update(dt);

	return UPDATE_CONTINUE;
}


bool ModuleCamera::CleanUp()
{
	return true;
}

void ModuleCamera::SetFOV(float verticalFov)
{
	activeCamera->SetFOV(verticalFov);
	//this->verticalFov = verticalFov;
	//frustum.SetVerticalFovAndAspectRatio(verticalFov, this->aspectRatio);
}

void ModuleCamera::SetAspectRatio(float aspectRatio)
{
	activeCamera->SetAspectRatio(aspectRatio);
	//this->aspectRatio = aspectRatio;
	//horizontalFov = 2 * atan(tan(verticalFov * 0.5) * aspectRatio);
	//frustum.SetHorizontalFovAndAspectRatio(this->horizontalFov, aspectRatio);
}

void ModuleCamera::ChangeWindowSize(int width, int height)
{
	activeCamera->ChangeWindowSize(width, height);
	//App->window->screen_width = width;
	//App->window->screen_height = height;
	//aspectRatio = (float)width / (float)height;
	//SetAspectRatio(aspectRatio);
	//glViewport(0, 0, width, height);
}

void ModuleCamera::SetPlaneDistances(float near, float far)
{
	activeCamera->SetPlaneDistances(near, far);
	//this->nearPlane = near;
	//this->farPlane = far;
	//frustum.SetViewPlaneDistances(near, far);
}

void ModuleCamera::Move(float dt)
{
	activeCamera->Move(dt);
	//float3 movement = float3::zero;

	//if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	//	movement -= frustum.WorldRight();
	//if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	//	movement += frustum.WorldRight();

	//if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	//	movement += frustum.Front();
	//if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	//	movement -= frustum.Front();

	//if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT)
	//	movement += float3::unitY;
	//if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT)
	//	movement -= float3::unitY;

	//movementSpeed = 5.0f;

	//if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
	//	movement *= 3;


	//if (movement.Equals(float3::zero) == false)
	//{
	//	frustum.Translate(movement * movementSpeed * dt / 1000);
	//	//This line below is required so viewmatrix is actualized.
	//	SetPosition(frustum.Pos());
	//}
}


void ModuleCamera::Zoom(float dt)
{
	activeCamera->Zoom(dt);
	//float3 movement = float3::zero;
	//float zoomSpeed = 20.0f;
	//int mouseWheel = App->input->GetMouseWheel();
	//
	//if (mouseWheel != 0 && mouseBlocked == false) {
	//	movement += frustum.Front() * mouseWheel * zoomSpeed * dt / 1000;
	//}

	//if (movement.Equals(float3::zero) == false)
	//{
	//	frustum.Translate(movement);
	//	//This line below is required so viewmatrix is actualized.
	//	SetPosition(frustum.Pos());
	//}
}


void ModuleCamera::Rotate(float dt)
{
	activeCamera->Rotate(dt);
	//Quat rotation = Quat::identity;

	//rotationSpeed = 2.0f;
	//if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
	//	rotationSpeed *= 3;

	//float angleX = 0;
	//float angleY = 0;
	//float angleChange = rotationSpeed * (dt / 1000);

	//fPoint mouseMotion = App->input->GetMouseMotion();
	//if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT && mouseBlocked == false)
	//{
	//	if (invertYAxis) {
	//		angleX += (mouseMotion.y / 4) * angleChange;
	//	}
	//	else {
	//		angleX -= (mouseMotion.y / 4) * angleChange;
	//	}
	//	if (invertXAxis) {
	//		angleY += (mouseMotion.x / 4) * angleChange;
	//	}
	//	else {
	//		angleY -= (mouseMotion.x / 4) * angleChange;
	//	}
	//}

	//if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	//	angleY += angleChange;
	//if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	//	angleY -= angleChange;
	//if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	//	angleX += angleChange;
	//if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	//	angleX -= angleChange;

	//if (angleY != 0)
	//{
	//	rotation = Quat::RotateY(angleY);
	//	frustum.SetFront(rotation.Transform(frustum.Front()));
	//	frustum.SetUp(rotation.Transform(frustum.Up()));
	//}
	//if (angleX != 0)
	//{
	//	rotation = Quat::RotateAxisAngle(frustum.WorldRight(), (angleX));
	//	frustum.SetFront(rotation.Transform(frustum.Front()));
	//	frustum.SetUp(rotation.Transform(frustum.Up()));
	//}

}

void ModuleCamera::SetPosition(const math::vec &pos)
{
	activeCamera->SetPosition(pos);
	//this->pos = pos;
	//frustum.SetPos(pos);
}

// TODO An assert must check Front and Up vectors are in a 90 degree angle
void ModuleCamera::SetLookAt(const math::vec & up, const math::vec & front)
{
	activeCamera->SetLookAt(up, front);
	//frustum.SetUp(up);
	//frustum.SetFront(front);
}

float * ModuleCamera::GetMatrixProjection() const
{
	return activeCamera->GetMatrixProjection();
	//float4x4 projectionMatrix = frustum.ProjectionMatrix();
	//float4x4 m = projectionMatrix.Transposed();
	//return &(m[0][0]);
}

float * ModuleCamera::GetMatrixView() const
{
	return activeCamera->GetMatrixView();
	//float4x4 viewMatrix = frustum.ViewMatrix();
	//float4x4 m = viewMatrix.Transposed();
	//return &(m[0][0]);
}

ComponentCamera* ModuleCamera::GetActiveCamera() const
{
	return activeCamera;
}