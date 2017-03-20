#ifndef COMPONENTSCRIPT_H
#define COMPONENTSCRIPT_H

#include "Component.h"
class ComponentScript :
	public Component
{
public:
	ComponentScript(GameObject* container, std::string id);
	~ComponentScript();

	bool DrawGUI();
};

#endif // !COMPONENTSCRIPT_H