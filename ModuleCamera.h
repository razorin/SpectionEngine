#ifndef MODULE_CAMERA_H
#define MODULE_CAMERA_H

#include "MathGeoLib\include\MathGeoLib.h"
#include "Module.h"
#include "MathGeoLib/include/MathGeoLib.h"

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
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();
	void ChangeWindowSize(int width, int height);
	void SetFOV(float verticalFov);
	void SetAspectRatio(float aspectRatio);
	void SetPlaneDistances(float near, float far);
	float4x4 GetMatrixProjection() const;
	float3x4 GetMatrixView() const;
	void SetPosition(const math::vec &pos);
	void SetOrientation(Axis axis, float rotation);
	void SetLookAt(const math::vec &up, const math::vec &front);

private:
	Frustum *frustum = nullptr;
	float aspectRatio = 1.78;
	float verticalFov = 30;
	float horizontalFov = 30;
	float near = 0;
	float far = 0;

	math::vec pos = { 0,0,0 };

};

#endif // !MODULE_CAMERA_H
