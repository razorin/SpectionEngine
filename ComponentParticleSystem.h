#ifndef COMPONENT_PARTICLE_SYSTEM_H
#define COMPONENT_PARTICLE_SYSTEM_H

#include "Component.h"
#include "ComponentBillboarding.h"
#include "AssimpIncludes.h"

struct Particle {
	aiVector3D position;
	aiVector3D velocity;
	unsigned lifetime;
};

class ComponentParticleSystem :
	public Component
{
private:
	typedef std::vector<Billboard> BillboardList;
	typedef std::vector<Particle> ParticlePool;
	typedef std::vector<unsigned> ParticleList;

public:
	ComponentParticleSystem(GameObject* container, std::string id);
	~ComponentParticleSystem();

	void Init(unsigned maxParticles, const aiVector2D& emitArea, unsigned fallingTime, float fallingHeight, const char* textureFile, const aiVector2D particleSize);
	void Clear();
	void Update(unsigned elapsed, const float3 cameraPosition);
	void Draw();
	bool DrawGUI();

private:
	ParticlePool particles;
	ParticleList alive;
	ParticleList dead;
	BillboardList billboards;

	aiVector2D emitArea;
	unsigned fallingTime = 0;
	unsigned accumElapsed = 0;
	float fallingHeight = 0.0f;
	unsigned texture = 0;

	aiVector3D* vertices = nullptr;
	aiVector2D* textureCoords = nullptr;
	aiColor4D* colors = nullptr;
	unsigned* indices = nullptr;
};

#endif // !COMPONENT_PARTICLE_SYSTEM_H