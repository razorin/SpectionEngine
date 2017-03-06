#ifndef __MODULEPARTICLES_H__
#define __MODULEPARTICLES_H__

#include<list>
#include "Module.h"
#include "Animation.h"
#include "Point.h"
#include "ModuleCollision.h"
#include "MemLeaks.h"

struct SDL_Texture;

enum PARTICLE_TYPE {
	LASER,
	EXPLOSION
};

struct Particle
{

	iPoint speed;
	iPoint position;
	float time_to_live;
	float start_time;
	bool has_collisions;
	bool auto_destroy;
	Animation animation;
	int fx;
	PARTICLE_TYPE type;

	SDL_Rect area;
	Collider *collider = nullptr;
	COLLIDER_TYPE collider_type;
	


	Particle();
	Particle(const Particle& p);
	~Particle();
	bool Update(float dt);
};

class ModuleParticles : public Module
{
public:
	ModuleParticles(const JSON_Object *json = nullptr);
	~ModuleParticles();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void AddParticle(const Particle& particle, int x, int y); // feel free to expand this call

private:

	std::list<Particle*> active;

public:
	SDL_Texture* graphics = nullptr;
	//Particle laser_particle;
	//Particle explosion_particle;
	// prototype particles go here ...
};

#endif // __MODULEPARTICLES_H__