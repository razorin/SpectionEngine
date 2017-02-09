#ifndef MODULE_CAMERA_H
#define MODULE_CAMERA_H

#include "Module.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "MemLeaks.h"

enum Axis {
	X,
	Y,
	Z
};

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
	void Rotate(float dt);
	void SetPosition(const math::vec &pos);
	void SetOrientation(Axis axis, float rotation);
	void SetLookAt(const math::vec &up, const math::vec &front);
	void RotateCamera(Axis axis, float rotation);

	float * GetMatrixProjection() const;
	float * GetMatrixView() const;

private:
	Frustum frustum;
	float aspectRatio = 1.78;
	float verticalFov = 30;
	float horizontalFov = 30;
	float near = 0;
	float far = 0;

	math::vec pos = { 0,0,0 };

};

#endif // !MODULE_CAMERA_H
