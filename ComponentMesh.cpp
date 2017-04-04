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
	name = "Mesh";
}

ComponentMesh::ComponentMesh(GameObject * container, ComponentMesh * component) : Component(component, container)
{
	if (component->mesh != nullptr) {
		mesh = new Mesh(component->mesh);
	}
}

ComponentMesh::~ComponentMesh()
{
}

void ComponentMesh::DrawMesh() {
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vboVertices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);



	if (mesh->normals != nullptr)
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->vboNormals);
		glNormalPointer(GL_FLOAT, sizeof(float) * 3, 0);
	}

	if (mesh->colors != nullptr)
	{
		glEnableClientState(GL_COLOR_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->vboColors);
		glColorPointer(3, GL_FLOAT, 0, NULL);
	}

	if (mesh->textureCoords != nullptr)
	{
		//for (int i = 0; i < numTextures; i++) {
		//	glBindTexture(GL_TEXTURE_2D, imageName);
		//	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		//	glBindBuffer(GL_ARRAY_BUFFER, vboTextures[i]);
		//	//Deberia estar fuera
		//	//glTexCoordPointer(3, GL_FLOAT, 0, NULL);
		//}
		glTexCoordPointer(3, GL_FLOAT, 0, NULL);
		glBindTexture(GL_TEXTURE_2D, mesh->imageName);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->vboTextures);
		glTexCoordPointer(3, GL_FLOAT, 0, NULL);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->vboIndices);
	glDrawElements(GL_TRIANGLES, mesh->numIndices, GL_UNSIGNED_INT, NULL);


	glBindTexture(GL_TEXTURE_2D, 0);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

bool ComponentMesh::DrawGUI()
{
	std::string headerLabel = name + "##" + id;
	if (ImGui::CollapsingHeader(headerLabel.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		std::string checkboxLabel = "Active##" + id;
		ImGui::Checkbox(checkboxLabel.c_str(), &active);
	}
	std::string removeLabel = "Remove Component##" + id;
	if (ImGui::Button(removeLabel.c_str())) {
		toDelete = true;
	}
	return false;
}