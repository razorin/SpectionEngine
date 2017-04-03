#ifndef MODULE_CAMERA_H
#define MODULE_CAMERA_H

#include "Module.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "MemLeaks.h"

class ComponentCamera;

class ModuleCamera :
	public Module
{
public:
	ModuleCamera(const JSON_Object *json);
	~ModuleCamera();
	bool Init();
	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	//Projection Related Functions
	void ChangeWindowSize(int width, int height);
	void SetFOV(float verticalFov);
	void SetAspectRatio(float aspectRatio);
	void SetPlaneDistances(float near, float far);

	//View Related Functions
	void Move(float dt);
	void Zoom(float dt);
	void Rotate(float dt);
	void SetPosition(const math::vec &pos);
	void SetLookAt(const math::vec &up, const math::vec &front);

	float * GetMatrixProjection() const;
	float * GetMatrixView() const;

	ComponentCamera* GetActiveCamera() const;

public:
	//These two params may be added as json config
	float movementSpeed = NULL;
	float rotationSpeed = NULL;

	bool invertXAxis = false;
	bool invertYAxis = false;

	math::vec pos = { 0,0,0 };
private:
	ComponentCamera *activeCamera = nullptr;
	//std::list<ComponentCamera *> cameras;
	Frustum frustum;
	float aspectRatio = 1.78;
	float verticalFov = 30;
	float horizontalFov = 30;
	float nearPlane = 0;
	float farPlane = 0;


	bool mouseBlocked = false;
};

#endif // !MODULE_CAMERA_H
