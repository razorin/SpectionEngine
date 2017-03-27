#include "Globals.h"
#include "Application.h"
#include "ModuleGUI.h"
#include "ModuleRender.h"
#include "ModuleCamera.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "ModuleSceneManager.h"
#include "Parson.h"
#include "SPrimitive.h"
#include "SCube.h"
#include "SPlane.h"
#include "SCylinder.h"
#include "LightsManager.h"
#include "Light.h"
#include "SDL/include/SDL.h"
#include <math.h>
#include "MathGeoLib/include/MathGeoLib.h"
#include "IMGUI\imgui.h"
#include "IMGUI\imgui_impl_sdl_gl3.h"


ModuleRender::ModuleRender(const JSON_Object *json) : Module(json)
{

	vsync = json_object_dotget_boolean(json, "vsync");
}

// Destructor
ModuleRender::~ModuleRender()
{}

// Called before render is available
bool ModuleRender::Init()
{
	App->gui->console.AddLog("Creating Renderer context");
	DLOG("Creating Renderer context");
	bool ret = true;


	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	//Create OpenGL Context
	context = SDL_GL_CreateContext(App->window->window);
	if (context == NULL)
	{
		App->gui->console.AddLog("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		DLOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	// Init Glew
	GLenum err = glewInit();
	//Checking errors
	if (err != GLEW_OK)
	{
		App->gui->console.AddLog("Error on glewInit: %s", glewGetErrorString(err));
		DLOG("Error on glewInit: %s", glewGetErrorString(err));
		ret = false;
	}
	else
	{
		// Should be 2.0
		App->gui->console.AddLog("Using Glew %s", glewGetString(GLEW_VERSION));
		DLOG("Using Glew %s", glewGetString(GLEW_VERSION));
	}


	if (ret == true)
	{
		GetHWAndDriverCapabilities();

		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
		//glEnable(GL_CULL_FACE);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);
		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);
	}

	return ret;
}

update_status ModuleRender::PreUpdate(float dt)
{
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glLoadMatrixf(App->camera->GetMatrixProjection());
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLoadMatrixf(App->camera->GetMatrixView());

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update(float dt)
{
	glBindTexture(GL_TEXTURE_2D, 0);
	DrawGrid();
	//DrawGizmo();

	App->lightsManager->Draw();

	// Draw the current scene
	if (App->sceneManager->getCurrentScene() != nullptr) App->sceneManager->Draw();

	//DrawDirectCube();

	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate(float dt)
{
	//SDL_RenderPresent(renderer);

	ImGui::Render();

	//Swap Buffer (OpenGL)
	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	App->gui->console.AddLog("Destroying renderer");
	DLOG("Destroying renderer");

	SDL_GL_DeleteContext(context);

	//Destroy window
	if (renderer != nullptr)
	{
		SDL_DestroyRenderer(renderer);
	}

	return true;
}

void ModuleRender::DrawGrid()
{
	//Draw Grid
	glBegin(GL_LINES);
	glLineWidth(2.0f);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	float gridLines = 50;


	for (int i = 0; i < gridLines; i++)
	{
		glVertex3f(-(gridLines / 2) + i, 0, -gridLines / 2);
		glVertex3f(-(gridLines / 2) + i, 0, gridLines / 2);
	}
	for (int i = 0; i < gridLines; i++)
	{
		glVertex3f(-gridLines / 2, 0, -(gridLines / 2) + i);
		glVertex3f(gridLines / 2, 0, -(gridLines / 2) + i);
	}
	glEnd();
	glLineWidth(1.0f);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void ModuleRender::DrawGizmo()
{
	//Draw Gyzmo
	glBegin(GL_LINES);
	glLineWidth(5.0f);
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f); // Red
	//Line
	glVertex3f(0, 0, 0);
	glVertex3f(1, 0, 0);
	//Letter
	glVertex3f(1.2f, 0.1f, 0.1f);
	glVertex3f(1.2f, -0.1f, -0.1f);
	glVertex3f(1.2f, 0.1f, -0.1f);
	glVertex3f(1.2f, -0.1f, 0.1f);

	glColor4f(0.0f, 1.0f, 0.0f, 1.0f); // Green
	//Line
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1, 0);
	//Letter
	glVertex3f(0.2f, 1.2f, 0);
	glVertex3f(0, 1.2f, 0);
	glVertex3f(0, 1.2f, 0);
	glVertex3f(-0.1f, 1.2f, -0.1f);
	glVertex3f(0, 1.2f, 0);
	glVertex3f(-0.1f, 1.2f, 0.1f);

	glColor4f(0.0f, 0.0f, 1.0f, 1.0f); // Blue
	//Line
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 1);
	//Letter
	glVertex3f(0.1f, 0.1f, 1.2f);
	glVertex3f(-0.1f, 0.1f, 1.2f);
	glVertex3f(-0.1f, 0.1f, 1.2f);
	glVertex3f(0.1f, -0.1f, 1.2f);
	glVertex3f(0.1f, -0.1f, 1.2f);
	glVertex3f(-0.1f, -0.1f, 1.2f);

	glEnd();
	glLineWidth(1.0f);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void ModuleRender::DrawDirectCube()
{

	float cubeSize = 0.5;
	//Draw Cube
	glBegin(GL_TRIANGLES);
	//Front
	glVertex3f(-cubeSize, -cubeSize, cubeSize);
	glVertex3f(cubeSize, -cubeSize, cubeSize);
	glVertex3f(-cubeSize, cubeSize, cubeSize);

	glVertex3f(-cubeSize, cubeSize, cubeSize);
	glVertex3f(cubeSize, -cubeSize, cubeSize);
	glVertex3f(cubeSize, cubeSize, cubeSize);

	//Right
	glVertex3f(cubeSize, -cubeSize, cubeSize);
	glVertex3f(cubeSize, -cubeSize, -cubeSize);
	glVertex3f(cubeSize, cubeSize, cubeSize);

	glVertex3f(cubeSize, cubeSize, cubeSize);
	glVertex3f(cubeSize, -cubeSize, -cubeSize);
	glVertex3f(cubeSize, cubeSize, -cubeSize);


	//Left
	glVertex3f(-cubeSize, -cubeSize, -cubeSize);
	glVertex3f(-cubeSize, -cubeSize, cubeSize);
	glVertex3f(-cubeSize, cubeSize, -cubeSize);

	glVertex3f(-cubeSize, cubeSize, -cubeSize);
	glVertex3f(-cubeSize, -cubeSize, cubeSize);
	glVertex3f(-cubeSize, cubeSize, cubeSize);


	//Back
	glVertex3f(cubeSize, -cubeSize, -cubeSize);
	glVertex3f(-cubeSize, -cubeSize, -cubeSize);
	glVertex3f(cubeSize, cubeSize, -cubeSize);

	glVertex3f(cubeSize, cubeSize, -cubeSize);
	glVertex3f(-cubeSize, -cubeSize, -cubeSize);
	glVertex3f(-cubeSize, cubeSize, -cubeSize);


	//Top
	glVertex3f(-cubeSize, cubeSize, cubeSize);
	glVertex3f(cubeSize, cubeSize, cubeSize);
	glVertex3f(-cubeSize, cubeSize, -cubeSize);

	glVertex3f(-cubeSize, cubeSize, -cubeSize);
	glVertex3f(cubeSize, cubeSize, cubeSize);
	glVertex3f(cubeSize, cubeSize, -cubeSize);


	//Bottom
	glVertex3f(-cubeSize, -cubeSize, -cubeSize);
	glVertex3f(cubeSize, -cubeSize, -cubeSize);
	glVertex3f(-cubeSize, -cubeSize, cubeSize);

	glVertex3f(-cubeSize, -cubeSize, cubeSize);
	glVertex3f(cubeSize, -cubeSize, -cubeSize);
	glVertex3f(cubeSize, -cubeSize, cubeSize);

	glEnd();

}

void ModuleRender::GetHWAndDriverCapabilities()
{
	DLOG("Vendor: %s", glGetString(GL_VENDOR));
	DLOG("Renderer: %s", glGetString(GL_RENDERER));
	DLOG("OpenGL version supported %s", glGetString(GL_VERSION));
	DLOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
}
