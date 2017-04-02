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
	movementSpeed = 5.0f;
	rotationSpeed = .1f;
}


ModuleCamera::~ModuleCamera()
{
	RELEASE(editorCamera);
}

bool ModuleCamera::Init()
{

	editorCamera = new ComponentCamera(nullptr, "");

	return true;
}

bool ModuleCamera::Start()
{
	return true;
}

update_status ModuleCamera::Update(float dt)
{
	mouseBlocked = App->gui->uiInput;
	
	editorCamera->SetMouseBlocked(mouseBlocked);
	float3 movement = float3::zero;
	float angleX = 0;
	float angleY = 0;

	Move(dt, movement);
	Zoom(dt, movement);
	Rotate(dt, angleX, angleY);

	editorCamera->Traslate(movement, angleX, angleY);

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
	editorCamera->SetFOV(verticalFov);
}

void ModuleCamera::SetAspectRatio(float aspectRatio)
{
	editorCamera->SetAspectRatio(aspectRatio);
}

void ModuleCamera::ChangeWindowSize(int width, int height)
{
	editorCamera->ChangeWindowSize(width, height);
}

void ModuleCamera::SetPlaneDistances(float near, float far)
{
	editorCamera->SetPlaneDistances(near, far);
}

void ModuleCamera::Move(float dt, float3 &movement)
{
	//float3 movement = float3::zero;
	float speed = movementSpeed;

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		movement -= editorCamera->frustum.WorldRight();
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		movement += editorCamera->frustum.WorldRight();

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		movement += editorCamera->frustum.Front();
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		movement -= editorCamera->frustum.Front();

	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT)
		movement += float3::unitY;
	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT)
		movement -= float3::unitY;


	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed *= 3;

	movement = movement * speed * dt / 1000;
}


void ModuleCamera::Zoom(float dt, float3 &movement)
{
	//float3 movement = float3::zero;
	float zoomSpeed = 20.0f;
	int mouseWheel = App->input->GetMouseWheel();
	
	if (mouseWheel != 0 && mouseBlocked == false) {
		movement += editorCamera->frustum.Front() * mouseWheel * zoomSpeed * dt / 1000;
	}

}


void ModuleCamera::Rotate(float dt, float &angleX, float &angleY)
{
	fPoint mouseMotion = App->input->GetMouseMotion();
	float angleChange = rotationSpeed * (dt / 1000);

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT && mouseBlocked == false) {
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
}

void ModuleCamera::SetPosition(const math::vec &pos)
{
	editorCamera->SetPosition(pos);
	//this->pos = pos;
	//frustum.SetPos(pos);
}

// TODO An assert must check Front and Up vectors are in a 90 degree angle
void ModuleCamera::SetLookAt(const math::vec & up, const math::vec & front)
{
	editorCamera->SetLookAt(up, front);
	//frustum.SetUp(up);
	//frustum.SetFront(front);
}

float * ModuleCamera::GetMatrixProjection() const
{
	return editorCamera->GetMatrixProjection();
	//float4x4 projectionMatrix = frustum.ProjectionMatrix();
	//float4x4 m = projectionMatrix.Transposed();
	//return &(m[0][0]);
}

float * ModuleCamera::GetMatrixView() const
{
	return editorCamera->GetMatrixView();
	//float4x4 viewMatrix = frustum.ViewMatrix();
	//float4x4 m = viewMatrix.Transposed();
	//return &(m[0][0]);
}

ComponentCamera* ModuleCamera::GetEditorCamera() const
{
	return editorCamera;
}

ComponentCamera* ModuleCamera::GetActiveCamera() const {
	return activeCamera;
}

void ModuleCamera::AddCamera(ComponentCamera *camera) {
	if (camera->active == true)
		activeCamera = camera;
	cameras.push_back(camera);
}
void ModuleCamera::RemoveCamera(ComponentCamera *camera) {
	if (camera->active)
		activeCamera = nullptr;
	cameras.remove(camera);
}
void ModuleCamera::ChangeActiveCamera(ComponentCamera *camera) {
	for (std::list<ComponentCamera*>::iterator it = cameras.begin(); it != cameras.end(); ++it) {
		if ((*it) == camera) {
			(*it)->active = true;
		}
		else {
			(*it)->active = false;
		}
	}
}