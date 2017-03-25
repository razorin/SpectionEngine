#include "ModuleCamera.h"
#include "SDL/include/SDL.h"
#include "Application.h"
#include "ModuleGUI.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "ComponentCamera.h"
#include "Parson.h"
#include <math.h>
#include <list>


ModuleCamera::ModuleCamera(const JSON_Object *json) : Module(json)
{
	invertXAxis = (bool)json_object_dotget_boolean(json, "invertHorizontalCamera");
	invertYAxis = (bool)json_object_dotget_boolean(json, "invertVerticalCamera");
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
	Move(dt);
	Zoom(dt);
	Rotate(dt);
	
	// Print position and orientation
	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) {
		App->gui->console.AddLog("Camera position: %f, %f, %f", pos.x, pos.y, pos.z);
		DLOG("Camera position: %f, %f, %f", pos.x, pos.y, pos.z);
	}
	if (App->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN) {
		App->gui->console.AddLog("Camera UP vector: %f, %f, %f", editorCamera->frustum.Up().x, editorCamera->frustum.Up().y, editorCamera->frustum.Up().z);
		DLOG("Camera UP vector: %f, %f, %f", editorCamera->frustum.Up().x, editorCamera->frustum.Up().y, editorCamera->frustum.Up().z);
		App->gui->console.AddLog("Camera FRONT vector: %f, %f, %f", editorCamera->frustum.Front().x, editorCamera->frustum.Front().y, editorCamera->frustum.Front().z);
		DLOG("Camera FRONT vector: %f, %f, %f", editorCamera->frustum.Front().x, editorCamera->frustum.Front().y, editorCamera->frustum.Front().z);
	}

	return UPDATE_CONTINUE;
}


bool ModuleCamera::CleanUp()
{
	return true;
}

void ModuleCamera::ChangeWindowSize(int width, int height)
{
	//editorCamera->ChangeWindowSize(width, height);
	App->window->screen_width = width;
	App->window->screen_height = height;
	for (std::list<ComponentCamera*>::iterator it = cameras.begin(); it != cameras.end(); ++it) {
		(*it)->ChangeWindowSize(width, height);
	}
	glViewport(0, 0, width, height);
}

void ModuleCamera::Move(float dt)
{
	//editorCamera->Move(dt);
	float3 movement = float3::zero;

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

	movementSpeed = 5.0f;

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		movement *= 3;


	if (movement.Equals(float3::zero) == false)
	{
		editorCamera->frustum.Translate(movement * movementSpeed * dt / 1000);
		//This line below is required so viewmatrix is actualized.
		SetPosition(editorCamera->frustum.Pos());
	}
}


void ModuleCamera::Zoom(float dt)
{
	//editorCamera->Zoom(dt);
	float3 movement = float3::zero;
	float zoomSpeed = 20.0f;
	int mouseWheel = App->input->GetMouseWheel();
	
	if (mouseWheel != 0 && mouseBlocked == false) {
		movement += editorCamera->frustum.Front() * mouseWheel * zoomSpeed * dt / 1000;
	}

	if (movement.Equals(float3::zero) == false)
	{
		editorCamera->frustum.Translate(movement);
		//This line below is required so viewmatrix is actualized.
		SetPosition(editorCamera->frustum.Pos());
	}
}


void ModuleCamera::Rotate(float dt)
{
	//editorCamera->Rotate(dt);
	Quat rotation = Quat::identity;

	rotationSpeed = 2.0f;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		rotationSpeed *= 3;

	float angleX = 0;
	float angleY = 0;
	float angleChange = rotationSpeed * (dt / 1000);

	fPoint mouseMotion = App->input->GetMouseMotion();
	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT && mouseBlocked == false)
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
		editorCamera->frustum.SetFront(rotation.Transform(editorCamera->frustum.Front()));
		editorCamera->frustum.SetUp(rotation.Transform(editorCamera->frustum.Up()));
	}
	if (angleX != 0)
	{
		rotation = Quat::RotateAxisAngle(editorCamera->frustum.WorldRight(), (angleX));
		editorCamera->frustum.SetFront(rotation.Transform(editorCamera->frustum.Front()));
		editorCamera->frustum.SetUp(rotation.Transform(editorCamera->frustum.Up()));
	}
}

void ModuleCamera::SetPosition(const math::vec &pos)
{
	//editorCamera->SetPosition(pos);
	this->pos = pos;
	editorCamera->frustum.SetPos(pos);
}

// TODO An assert must check Front and Up vectors are in a 90 degree angle
void ModuleCamera::SetLookAt(const math::vec & up, const math::vec & front)
{
	//editorCamera->SetLookAt(up, front);
	editorCamera->frustum.SetUp(up);
	editorCamera->frustum.SetFront(front);
}

float * ModuleCamera::GetMatrixProjection() const
{
	//return editorCamera->GetMatrixProjection();
	float4x4 projectionMatrix = editorCamera->frustum.ProjectionMatrix();
	float4x4 m = projectionMatrix.Transposed();
	return &(m[0][0]);
}

float * ModuleCamera::GetMatrixView() const
{
	//return editorCamera->GetMatrixView();
	float4x4 viewMatrix = editorCamera->frustum.ViewMatrix();
	float4x4 m = viewMatrix.Transposed();
	return &(m[0][0]);
}

void ModuleCamera::AddCamera(ComponentCamera* cam) {
	cameras.push_back(cam);
}

void ModuleCamera::RemoveCamera(ComponentCamera* cam) {
	cameras.remove(cam);
}

ComponentCamera* ModuleCamera::GetEditorCamera() const
{
	return editorCamera;
}