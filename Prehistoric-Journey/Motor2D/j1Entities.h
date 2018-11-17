#ifndef __j1ENTITIES_H__
#define __j1ENTITIES_H__

#include "j1Module.h"
#include "Animation.h"
#include "p2DynArray.h"

class Entity;
class Player;
struct SDL_Texture;

enum class EntityTypes
{
	PLAYER,
	TREX,
	MINI_TREX,
	PTERODACTYL,
	BAT,
	UNKNOWN
};

class j1Entities : public j1Module
{
public:

	j1Entities();
	~j1Entities();

	bool Awake(pugi::xml_node& conf);

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool CleanUp();

	bool SpawnEntity(int x, int y, EntityTypes type);

	void OnCollision(Collider* c1, Collider* c2);

	Player* GetPlayer() const;

	bool Load(pugi::xml_node& entity_node);

	bool Save(pugi::xml_node& entity_node) const;
	
public:

	p2DynArray<Entity*> entities;

private:
	
};



#endif // !__j1ENTITIES_H__
