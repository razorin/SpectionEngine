#ifndef MODULE_CAMERA_H
#define MODULE_CAMERA_H


#include "Module.h"
class ModuleCamera :
	public Module
{
public:
	ModuleCamera();
	~ModuleCamera();
	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

};

#endif // !MODULE_CAMERA_H
