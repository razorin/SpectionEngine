#include <math.h>
#include "ModuleParticles.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"

#include "SDL/include/SDL_timer.h"

ModuleParticles::ModuleParticles(const JSON_Object *json) : Module(json)
{}

ModuleParticles::~ModuleParticles()
{}

// Load assets
bool ModuleParticles::Start()
{
	LOG("Loading particles");
	
	/*graphics = App->textures->Load("rtype/particles.png");


	// TODO 2: Create a prototype for the laser particle
	// audio: rtype/laser.wav
	// coords: {232, 103, 16, 12}; {249, 103, 16, 12};
	
	laser_particle.speed = { 5, 0 };
	//laser_particle.direction;
	laser_particle.time_to_live = 7000;
	laser_particle.auto_destroy = true;
	laser_particle.has_collisions = true;
	laser_particle.animation.frames.push_back({ 232, 103, 16, 12 });
	laser_particle.animation.frames.push_back({ 249, 103, 16, 12 });
	laser_particle.animation.speed = 0.1;
	laser_particle.fx = App->audio->LoadFx("rtype/laser.wav");
	laser_particle.type = PARTICLE_TYPE::LASER;
	laser_particle.area.h = 12;
	laser_particle.area.w = 16;

	laser_particle.collider = NULL;
	laser_particle.collider_type = COLLIDER_TYPE::PLAYER_BULLET;

	explosion_particle.speed = { 0, 0 };
	explosion_particle.time_to_live = 750;
	explosion_particle.auto_destroy = true;
	explosion_particle.has_collisions = false;

	explosion_particle.animation.frames.push_back({ 274, 296, 33, 30 }); 
	explosion_particle.animation.frames.push_back({ 313, 296, 33, 30 }); 
	explosion_particle.animation.frames.push_back({ 346, 296, 33, 30 }); 
	explosion_particle.animation.frames.push_back({ 382, 296, 33, 30 }); 
	explosion_particle.animation.frames.push_back({ 419, 296, 33, 30 }); 
	explosion_particle.animation.frames.push_back({ 457, 296, 33, 30 });
	explosion_particle.animation.loop = false;
	explosion_particle.animation.speed = .1f;

	explosion_particle.fx = App->audio->LoadFx("rtype/explosion.wav");
	explosion_particle.type = PARTICLE_TYPE::EXPLOSION;

	explosion_particle.collider = NULL;
	explosion_particle.collider_type = COLLIDER_TYPE::NONE;


	// TODO 12: Create a new "Explosion" particle 
	// audio: rtype/explosion.wav
	// coords: {274, 296, 33, 30}; {313, 296, 33, 30}; {346, 296, 33, 30}; {382, 296, 33, 30}; {419, 296, 33, 30}; {457, 296, 33, 30};
	*/
	return true;
}

// Unload assets
bool ModuleParticles::CleanUp()
{
	LOG("Unloading particles");
	App->textures->Unload(graphics);

	for (list<Particle*>::iterator it = active.begin(); it != active.end(); ++it)
		RELEASE(*it);

	active.clear();

	return true;
}

// Update: draw background
update_status ModuleParticles::Update()
{
	for (list<Particle*>::iterator it = active.begin(); it != active.end();)
	{
		Particle* p = *it;
		if(p->Update() == false)
		{
			RELEASE(*it);
			it = active.erase(it);
		}
		else
			++it;
	}

	return UPDATE_CONTINUE;
}

void ModuleParticles::AddParticle(const Particle& particle, int x, int y)
{
	// TODO 4: Fill in a method to create an instance of a prototype particle	
	Particle *p = new Particle(particle);
	p->area.x = x;
	p->area.y = y;
	p->position.x = x;
	p->position.y = y;
	//if(particle.has_collisions)
		//p->collider = App->collision->AddCollider(p->area, p->collider_type, std::bind(&Particle::OnCollision, p, std::placeholders::_1));
	active.push_back(p);

}

// -------------------------------------------------------------
// -------------------------------------------------------------

Particle::Particle()
{}

// TODO 3: Fill in a copy constructor
Particle::Particle(const Particle& p) 
{
	this->speed = p.speed;
	this->position = p.position;
	this->time_to_live = p.time_to_live;
	this->start_time = SDL_GetTicks();
	this->has_collisions = p.has_collisions;
	this->auto_destroy = p.auto_destroy;
	this->animation = p.animation;
	//this->animation.Reset();
	this->collider_type = p.collider_type;
	this->type = p.type;
	this->fx = p.fx;
	this->type = p.type;
	this->area = p.area;
	this->collider = nullptr;
}

Particle::~Particle()
{
	if (has_collisions && collider != nullptr)
		collider->to_delete = true;
	collider = nullptr;
}

bool Particle::Update()
{
	bool ret = true;

	// TODO 5: This is the core of the particle functionality.
	// Return false if the particle must be destroyed
	App->renderer->Blit(App->particles->graphics, position, &(animation.GetCurrentFrame()));
	if (fx >= 0) {
		App->audio->PlayFx(fx);
		fx = -1;
	}

	position += speed;
		
	if (has_collisions) {
		collider->rect.x = position.x;
		collider->rect.y = position.y;
	}
	
	return (!auto_destroy || auto_destroy && time_to_live >= SDL_GetTicks() - start_time);
}

/*void Particle::OnCollision(COLLIDER_TYPE collider_type) {
 	switch (this->type) {
		case PARTICLE_TYPE::LASER:
			if (collider_type == COLLIDER_TYPE::WALL)
				this->time_to_live = 0;
			App->particles->AddParticle(App->particles->explosion_particle, position.x, position.y);
			break;
	}
	
}
*/