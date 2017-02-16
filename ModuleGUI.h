#ifndef __MODULEGUI_H__
#define __MODULEGUI_H__

#include "Module.h"
#include<list>

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
	void AddFpsLog(float fps, float ms);
	void Clear();

public:
	bool ScrollToBottom;
	ImGuiTextBuffer* consoleBuffer = nullptr;
 
	int numFps = 0;
	std::list<float> fpsLog;
	int numMs = 0;
	std::list<float> msLog;
};

#endif // __MODULEGUI_H__