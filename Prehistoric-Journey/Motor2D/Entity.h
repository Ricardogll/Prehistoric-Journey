#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "p2Point.h"
#include "Animation.h"
#include "j1Entities.h"

struct SDL_Texture;
struct Collider;

class Entity {

public:
	
	Collider* collider;
	fPoint position = { 0.0f, 0.0f };
	EntityTypes type = EntityTypes::UNKNOWN;
	bool to_destroy = false;

public:
	Entity(int x, int y, EntityTypes type);
	virtual ~Entity();
	virtual bool Awake(pugi::xml_node&) { return true; }
	virtual void Update(float dt) {};
	virtual bool PostUpdate() { return true; }
	virtual void Draw();
	virtual void OnCollision(Collider* collider);
	virtual void AnimationsApplyDt() {};
	virtual bool Load(pugi::xml_node&)	{ return true; }
	virtual bool Save(pugi::xml_node&) const{ return true; }
};



#endif  // __ENTITY_H__