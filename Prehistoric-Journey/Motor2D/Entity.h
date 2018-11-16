#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "p2Point.h"
#include "Animation.h"
#include "j1Entities.h"
#include "j1Render.h"
#include "j1Textures.h"

struct SDL_Texture;
struct Collider;

enum X_DIRECTION {
	RIGHT,
	LEFT,
	NONE,
};

enum STATE {
	IDLE,
	RUN,
	JUMP,
	LIANA,
	LIANA_IDLE,
	ATTACK,
	NO_STATE,
};

class Entity {

public:
	
	Collider* collider;
	iPoint collider_dimensions = { 0,0 };
	iPoint collider_offset = { 0,0 };

	fPoint position = { 0.0f, 0.0f };
	fPoint speed = { 0.0f,0.0f };
	X_DIRECTION entity_x_dir = NONE;
	float moving_speed = 150.0f;
	float gravity = 0.0f;

	STATE state = NO_STATE;
	EntityTypes type = EntityTypes::UNKNOWN;
	
	Animation* current_animation = nullptr;
	Animation idle = Animation();
	Animation run = Animation();
	Animation death = Animation();
	float idle_anim_speed = 0.0f;
	float run_anim_speed = 0.0f;
	bool anim_speed_flag = false;

	SDL_Texture* texture = nullptr;
	p2SString spritesheet;
		
	const p2DynArray<iPoint>* path;

	bool to_destroy = false;
	float dt_current = 0.0f;
	

public:
	Entity(int x, int y, EntityTypes type);
	virtual ~Entity();
	virtual bool Awake(pugi::xml_node&) { return true; }
	virtual void Update(float dt) {};
	virtual void Draw();
	virtual void OnCollision(Collider* c1, Collider* c2);
	virtual void AnimationsApplyDt() {};
	virtual bool Load(pugi::xml_node&)	{ return true; }
	virtual bool Save(pugi::xml_node&) const{ return true; }
	void SetAnimations(pugi::xml_node& config, Animation& animation);
	fPoint SpeedNeededFromTo(iPoint& from, iPoint& to) const;
	void LoadVariablesXML(const pugi::xml_node& entity_node);
};



#endif  // __ENTITY_H__