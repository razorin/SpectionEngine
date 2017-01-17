#include "Player.h"
#include "SDL/include/SDL.h"
#include "Application.h"
#include "ModuleRender.h"
#include "Animation.h"
#include "ModuleInput.h"
#include "Globals.h"
#include "ModuleCollision.h"
#include "ModuleTextures.h"
#include "Parson.h"
#include "Timer.h"
#include "CodyIdleState.h"
#include "Enemy.h"
#include "Timer.h"

Player::Player(const JSON_Object *playerConfig) : Creature(playerConfig, ENTITY_TYPE::PLAYER){
	const char* path = json_object_dotget_string(playerConfig, "graphics");
	graphics =  App->textures->Load(path);

	state = new CodyIdleState();

	positionCollider = App->collision->AddCollider({position->x, position->y, 37, 88}, PLAYER_COLLIDER, false, false, std::bind(&Player::OnCollision, this, std::placeholders::_1));
	state->Start(this);
	hitsTimer = new Timer();
	currentEnemyTimer = new Timer();

	
	LOG("PLAYER CREATED");
}

void Player::Init(const iPoint &initialPosition) {
	active = true;
	originalAttack = attack;
	*position = initialPosition;
	positionCollider->SetPos(position->x + 35, position->y + 9, position->z);
}

Player::~Player() {
	currentAnimation = nullptr;
	RELEASE(hitsTimer);
}


void Player::Update() {
	previousPosition = iPoint(*position);
	speed.SetToZero();
	PlayerStateMachine *newState = state->Update(this);
	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN) {
		godMode = !godMode;
		if (godMode) {
			attack = 1000;
		}
		else {
			attack = originalAttack;
		}
	}
	if (newState != nullptr) {
		RELEASE(state);
		if (attackCollider != nullptr) {
			attackCollider->to_delete = true;
			attackCollider = nullptr;
		}
		state = newState;
		currentAnimation->Reset();
		state->Start(this);
	}
	if (hitsTimer->Ellapsed() > 2000)
		hits = 0;
	if (currentEnemyTimer->Ellapsed() > 2000)
		currentEnemy = nullptr;
	Move(speed);
}

void Player::OnCollision(const Collider &other) {
	if (!godMode) {
		iPoint newPosition = previousPosition - *position;
		switch (other.type) {
		case COLLIDER_TYPE::WALL:
		case COLLIDER_TYPE::CAMERA_WALL:
			if (previousPosition.x != position->x &&
				positionCollider->rect.x > other.rect.x && positionCollider->rect.x < other.rect.x + other.rect.w) {
				newPosition.x = 0;
			}
			//TODO: Now it's a little tricky. It needs a good revision to fix wall collisions
			if (previousPosition.y != position->y && other.ignore_y) {
				newPosition.y = 0;
			}
			newPosition.z = 0;
			Move(newPosition);
			break;
		case COLLIDER_TYPE::ENEMY_HIT:
			TakeDamage((Enemy*)other.owner);
			break;
		}
	}
}

void Player::TakeDamage(Enemy *enemy) {
	if (state != nullptr) {
		PlayerStateMachine *newState = positionCollider->z == 0 ? state->ChangeState(PLAYER_DAMAGE_STATE) : state->ChangeState(PLAYER_AIR_DAMAGE_STATE);
		if (newState != nullptr) {
			
			iPoint playerPoint = getBottomPoint();
			iPoint enemyPoint = enemy->getBottomPoint();
			int direction = playerPoint.x - enemyPoint.x;

			if (newState->GetState() == PLAYER_STATE::PLAYER_AIR_DAMAGE_STATE) {
				newState->direction = playerPoint.x - enemyPoint.x;
			}
			RELEASE(state);
			if (attackCollider != nullptr) {
				attackCollider->to_delete = true;
				attackCollider = nullptr;
			}
			state = newState;
			currentAnimation->Reset();
			state->Start(this);
			life -= enemy->attack;
			hits = 0;
		}
	}
}

void Player::AddHit() {
	LOG("Added hit!!! Current hit: %i", hits);
	if (hits == 0) {
		hitsTimer->Start();
		++hits;
	}
	else {
		if(hitsTimer->Ellapsed() - lastHitTime > 10)
			++hits;
		
	}
	if (hits > 3) {
		hits = 0;
		hitsTimer->Start();
	}
	lastHitTime = hitsTimer->Ellapsed();
}

void Player::AddCurrentEnemy(Enemy *enemy) {
	currentEnemy = enemy;
	currentEnemyTimer->Start();
}