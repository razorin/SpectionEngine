#ifndef MODULE_CAMERA_H
#define MODULE_CAMERA_H


#include "Module.h"
#include "MathGeoLib/include/MathGeoLib.h"


class ModuleCamera :
	public Module
{
public:
	ModuleCamera(const JSON_Object *json);
	~ModuleCamera();
	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();
	void ChangeWindowSize(int width, int height);
	void SetFOV(float verticalFov);
	void SetAspectRatio(float aspectRatio);
	void SetPlaneDistances(float near, float far);

private:
	Frustum *frustum = nullptr;
	float aspectRatio = 1.78;
	float verticalFov = 30;
	float horizontalFov = 30;
	float near = NULL;
	float far = NULL;

};

#endif // !MODULE_CAMERA_H
