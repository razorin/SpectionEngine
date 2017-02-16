#ifndef __MODULEGUI_H__
#define __MODULEGUI_H__

#include "Module.h"

struct ImGuiTextBuffer;

class ModuleGUI :
	public Module
{
public:
	ModuleGUI(const JSON_Object *json = nullptr);
	~ModuleGUI();
	
	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();
	void AddLog(const char* fmt, ...);
	void Draw(const char* title, bool* p_opened = NULL);
	void Clear();

public:
	bool ScrollToBottom;
	ImGuiTextBuffer* consoleBuffer = nullptr;
};

#endif // __MODULEGUI_H__