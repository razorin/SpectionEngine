#ifndef OCTREE_H
#define OCTREE_H

#include <vector>
#include "MathGeoLib\include\MathGeoLib.h"

class SPrimitive;
class GameObject;
#define MAX_NUMBER_OF_ELEMENTS_BY_NODE 1

class Octree
{
private:

	class OctreeNode {
	private:
		GameObject *element;
		math::AABB box;
		OctreeNode *parent = nullptr;
		std::vector<OctreeNode *> childs;
	public:
		void CalculateCandidates(std::vector<GameObject*> &candidates, const SPrimitive &primitive);
		void Insert(GameObject *element);
		void DivideNode();
		OctreeNode(const math::AABB & size, OctreeNode *parent = nullptr);
		bool isCandidate(const SPrimitive & primitive);
	};
public:
	Octree(const math::AABB &size);
	virtual ~Octree();
	//void Create(const math::AABB &size);
	void Insert(GameObject *go);
	//void Remove(T *element);
	void CalculateCandidates(std::vector<GameObject*> &candidates, const SPrimitive &primitive);
	void Draw();

	OctreeNode *root; 
	

};


bool Octree::OctreeNode::isCandidate(const SPrimitive & primitive)
{
	
	return false;
}

void Octree::OctreeNode::CalculateCandidates(std::vector<GameObject*>& candidates, const SPrimitive & primitive)
{
}

void Octree::OctreeNode::Insert(GameObject * element)
{
}

void Octree::OctreeNode::DivideNode()
{
	math::vec AABBoxPoints[8];	
	box.GetCornerPoints(AABBoxPoints);

}

Octree::OctreeNode::OctreeNode(const math::AABB & box, OctreeNode *parent) : box(box), parent(parent)
{
}

#endif // !OCTREE_H

