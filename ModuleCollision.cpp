#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
#include "Entity.h"

using namespace std;

ModuleCollision::ModuleCollision(const JSON_Object *json) : Module(json)
{
	matrix_collision = new bool[81]{false, false, false, false, false, false, false, false, false,
									false, false, false, true, true, true, true, true, true,
									false, false, false, true, false, false, false, true, false,
									false, false, true, false, false, true, false, true, false,
									false, true, false, false, false, false, false, false, false,
									false, true, false, true, false, false, false, false, false,
									false, true, false, false, false, false, false, false, false,
									false, true, true, true, false, false, false, false, false,
									false, true, false, false, false, false, false, false, false
									};
}

// Destructor
ModuleCollision::~ModuleCollision()
{}

update_status ModuleCollision::PreUpdate()
{
	// Remove all colliders scheduled for deletion
	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end();)
	{
		if ((*it)->to_delete == true)
		{
			RELEASE(*it);
			it = colliders.erase(it);
		}
		else
			++it;
	}

	return UPDATE_CONTINUE;
}

update_status ModuleCollision::Update()
{
	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); ++it){
		for (list<Collider*>::iterator jt = std::next(it); jt != colliders.end(); ++jt) {
			if (matrix_collision[(*it)->type * 9 + (*jt)->type] && (*it)->CheckCollision(**jt)) {
				(*it)->Notify((**jt));
				(*jt)->Notify((**it));
			}
		}
	}

	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(debug == true)
		DebugDraw();

	return UPDATE_CONTINUE;
}

void ModuleCollision::DebugDraw()
{
	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); ++it)
		App->renderer->DrawQuad((**it), 255, 0, 0, 80);
}

// Called before quitting
bool ModuleCollision::CleanUp()
{
	LOG("Freeing all colliders");

	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); ++it)
		RELEASE(*it);

	colliders.clear();

	return true;
}

Collider* ModuleCollision::AddCollider(const SDL_Rect& rect, COLLIDER_TYPE type, bool ignore_z, bool ignore_y, std::function<void(const Collider &)> onCollision, Entity *owner)
{
	Collider* ret = new Collider(rect, type, ignore_z, ignore_y, onCollision, owner);

	colliders.push_back(ret);

	return ret;
}

// -----------------------------------------------------

bool Collider::CheckCollision(const Collider &other) const
{
	// TODO 7: Create by hand (avoid consulting the internet) a simple collision test
	// Return true if the argument and the own rectangle are intersecting
	return (this->rect.x < other.rect.x + other.rect.w &&
		this->rect.x + this->rect.w > other.rect.x &&
		((this->ignore_y || other.ignore_y) ? true : this->rect.y + rect.h > other.rect.y &&  this->rect.y + rect.h <= other.rect.y + other.rect.h) &&
		((this->ignore_z || other.ignore_z) ? true : (this->z - z < 2 && this->z - z > -2)));

}

void Collider::Notify(const Collider &other) {
	if (onCollision)
		onCollision(other);
}