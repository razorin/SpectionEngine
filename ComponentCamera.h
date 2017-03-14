#ifndef COMPONENTCAMERA_H
#define COMPONENTCAMERA_H

#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "MemLeaks.h"

enum Axis {
	X,
	Y,
	Z
};

class ComponentCamera :
	public Component
{
public:
	ComponentCamera(GameObject* container);
	~ComponentCamera();

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
	void SetOrientation(Axis axis, float rotation);
	void SetLookAt(const math::vec &up, const math::vec &front);

	float * GetMatrixProjection() const;
	float * GetMatrixView() const;

	bool DrawGUI();

public:
	//These two params may be added as json config
	float movementSpeed = NULL;
	float rotationSpeed = NULL;

	bool invertXAxis = false;
	bool invertYAxis = false;


private:
	Frustum frustum;
	float aspectRatio = 1.78;
	float verticalFov = 30;
	float horizontalFov = 30;
	float nearPlane = 0;
	float farPlane = 0;

	math::vec pos = { 0,0,0 };

	bool mouseBlocked = false;
};

#endif // !COMPONENTCAMERA_H