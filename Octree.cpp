#include "Octree.h"
#include "Glew\include\GL\glew.h"


Octree::Octree(const math::AABB &size)
{
	root = new OctreeNode(size);
}

Octree::~Octree()
{
}

//template<class T>
//void Octree<T>::Create(const math::AABB & size)
//{
//}

void Octree::Insert(GameObject * element)
{
	
}

void Octree::CalculateCandidates(std::vector<GameObject*>& candidates, const SPrimitive & primitive)
{
}

 //TODO: Refactor this method and DrawBoundingBoxes of GO
void Octree::Draw()
{
	math::vec AABBoxPoints[8];

	glColor4f(1, 0, 1, 1);
	glBegin(GL_LINES);
	glVertex3f(
		AABBoxPoints[0].x,
		AABBoxPoints[0].y,
		AABBoxPoints[0].z);
	glVertex3f(
		AABBoxPoints[1].x,
		AABBoxPoints[1].y,
		AABBoxPoints[1].z);
	glVertex3f(
		AABBoxPoints[0].x,
		AABBoxPoints[0].y,
		AABBoxPoints[0].z);
	glVertex3f(
		AABBoxPoints[2].x,
		AABBoxPoints[2].y,
		AABBoxPoints[2].z);
	glVertex3f(
		AABBoxPoints[0].x,
		AABBoxPoints[0].y,
		AABBoxPoints[0].z);
	glVertex3f(
		AABBoxPoints[4].x,
		AABBoxPoints[4].y,
		AABBoxPoints[4].z);
	glVertex3f(
		AABBoxPoints[5].x,
		AABBoxPoints[5].y,
		AABBoxPoints[5].z);
	glVertex3f(
		AABBoxPoints[1].x,
		AABBoxPoints[1].y,
		AABBoxPoints[1].z);
	glVertex3f(
		AABBoxPoints[5].x,
		AABBoxPoints[5].y,
		AABBoxPoints[5].z);
	glVertex3f(
		AABBoxPoints[4].x,
		AABBoxPoints[4].y,
		AABBoxPoints[4].z);
	glVertex3f(
		AABBoxPoints[5].x,
		AABBoxPoints[5].y,
		AABBoxPoints[5].z);
	glVertex3f(
		AABBoxPoints[7].x,
		AABBoxPoints[7].y,
		AABBoxPoints[7].z);
	glVertex3f(
		AABBoxPoints[3].x,
		AABBoxPoints[3].y,
		AABBoxPoints[3].z);
	glVertex3f(
		AABBoxPoints[2].x,
		AABBoxPoints[2].y,
		AABBoxPoints[2].z);
	glVertex3f(
		AABBoxPoints[3].x,
		AABBoxPoints[3].y,
		AABBoxPoints[3].z);
	glVertex3f(
		AABBoxPoints[7].x,
		AABBoxPoints[7].y,
		AABBoxPoints[7].z);
	glVertex3f(
		AABBoxPoints[3].x,
		AABBoxPoints[3].y,
		AABBoxPoints[3].z);
	glVertex3f(
		AABBoxPoints[1].x,
		AABBoxPoints[1].y,
		AABBoxPoints[1].z);
	glVertex3f(
		AABBoxPoints[6].x,
		AABBoxPoints[6].y,
		AABBoxPoints[6].z);
	glVertex3f(
		AABBoxPoints[4].x,
		AABBoxPoints[4].y,
		AABBoxPoints[4].z);
	glVertex3f(
		AABBoxPoints[6].x,
		AABBoxPoints[6].y,
		AABBoxPoints[6].z);
	glVertex3f(
		AABBoxPoints[2].x,
		AABBoxPoints[2].y,
		AABBoxPoints[2].z);
	glVertex3f(
		AABBoxPoints[6].x,
		AABBoxPoints[6].y,
		AABBoxPoints[6].z);
	glVertex3f(
		AABBoxPoints[7].x,
		AABBoxPoints[7].y,
		AABBoxPoints[7].z);
	glEnd();
	glColor4f(1, 1, 1, 1);
}
