#ifndef __j1ENTITIES_H__
#define __j1ENTITIES_H__

#include "j1Module.h"
#include "Animation.h"
#include "p2DynArray.h"
#include "p2List.h"

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

struct ToCreate
{
	int x = 0;
	int y = 0;
	EntityTypes type=EntityTypes::UNKNOWN;

	ToCreate() {}
	ToCreate(int x, int y, EntityTypes type) :x(x), y(y), type(type) {}
	void Set(int x, int y, EntityTypes type) {
		this->x = x;
		this->y = y;
		this->type = type;
	}

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

	void SetEnemies(uint map);
	
public:

	p2DynArray<Entity*> entities;
	p2List<ToCreate> to_create1;
	p2List<ToCreate> to_create2;

private:
	
};



#endif // !__j1ENTITIES_H__
