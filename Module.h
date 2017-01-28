#ifndef __MODULE_H__
#define __MODULE_H__

#include "Globals.h"
#include "Glew/include/GL/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

class Application;

struct json_value_t;
typedef struct json_value_t  JSON_Value;
struct json_object_t;
typedef struct json_object_t  JSON_Object;



class Module
{
public:

	Module(const JSON_Object *json = nullptr, bool active = true) : active(active)
	{}

	Module(const JSON_Value *json = nullptr, bool active = true) : active(active) {
	}

	virtual ~Module()
	{}

	bool IsEnabled() const
	{
		return active;
	}

	 bool Enable()
	{
		if(active == false)
			return active = Start();

		return true;
	}

	 bool Disable()
	 {
		 if(active == true)
			 return active = !CleanUp();

		 return true;
	 }

	virtual bool Init() 
	{
		return true; 
	}

	virtual bool Start()
	{
		return true;
	}

	virtual update_status PreUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status Update(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp() 
	{ 
		return true; 
	}

	// Callbacks ---

private:
	bool active = true;
};

#endif // __MODULE_H__