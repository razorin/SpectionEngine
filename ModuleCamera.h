#ifndef MODULE_CAMERA_H
#define MODULE_CAMERA_H

#include "MathGeoLib\include\MathGeoLib.h"
#include "Module.h"


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
	void SetFOV(float verticalFov);
	void SetAspectRatio(float aspectRatio);
	void SetPlaneDistances(float near, float far);
	float4x4 GetMatrixProjection() const;
	float3x4 GetMatrixView() const;

private:
	Frustum *frustum = nullptr;
	float aspectRatio = 1.78;
	float verticalFov = 30;
	float horizontalFov = 30;
	float near = 0;
	float far = 0;
	float4x4 projectionMatrix = float4x4::identity;
	float3x4 viewMatrix = float3x4::identity;


};

#endif // !MODULE_CAMERA_H
