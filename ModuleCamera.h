#ifndef MODULE_CAMERA_H
#define MODULE_CAMERA_H


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
	void ChangeWindowSize(int width, int height);

};

#endif // !MODULE_CAMERA_H
