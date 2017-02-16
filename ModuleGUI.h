#ifndef __MODULEGUI_H__
#define __MODULEGUI_H__

#include "Module.h"


class ModuleGUI :
	public Module
{
public:
	ModuleGUI(const JSON_Object *json = nullptr);
	~ModuleGUI();
};

#endif // __MODULEGUI_H__