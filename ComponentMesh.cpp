#include "ComponentMesh.h"
#include "assimp\vector3.h"
#include "Glew/include/GL/glew.h"

//IMGUI Includes
#include "IMGUI\imconfig.h"
#include "IMGUI\imgui.h"
#include "IMGUI\imgui_impl_sdl_gl3.h"
#include "IMGUI\imgui_internal.h"
#include "IMGUI\stb_rect_pack.h"
#include "IMGUI\stb_textedit.h"
#include "IMGUI\stb_truetype.h"

ComponentMesh::ComponentMesh(GameObject * container, std::string id) :Component(container, ComponentType::COMPONENT_TYPE_MESH, id)
{
}

ComponentMesh::~ComponentMesh()
{
}

bool ComponentMesh::DrawGUI()
{
	return false;
}