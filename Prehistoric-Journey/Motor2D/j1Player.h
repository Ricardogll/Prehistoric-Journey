#ifndef _j1PLAYER_H_
#define _j1PLAYER_H_

#include	"PugiXml/src/pugixml.hpp"
#include	"j1Module.h"
#include	"p2List.h"
#include	"p2Point.h"
#include	"j1Textures.h"
#include	"Animation.h"

#define GRAVITY 0.1f 
#define SPEED_X 2.5f
#define SPEED_Y 0.25f


struct SDL_texture;
struct SDL_Rect;
struct Collider;
struct Mix_Chunk;


enum STATE {
	IDLE,
	RUN,
	JUMP,
	NO_STATE,
};

enum X_DIRECTION {
	RIGHT,
	LEFT,
	NONE,
};


class j1Player :public j1Module
{
public:
	j1Player();
	~j1Player();

	bool Start();
	bool CleanUp();
	bool Awake(pugi::xml_node& config);
	bool Update(float dt);
	bool PostUpdate();
	bool Save(pugi::xml_node&) const;
	bool Load(pugi::xml_node&);
	void LoadPosition();
	void SavePosition();

	void Draw();
	bool Jumping();
	void OnCollision(Collider* c1, Collider* c2);
private:

	SDL_Rect rect_player;

	uint width = 0u;
	uint height = 0u;
	SDL_Texture* texture = nullptr;
	fPoint speed;
	STATE state = NO_STATE;
	Animation* current_animation = nullptr;

	STATE last_state = NO_STATE;
	STATE last_state_2 = NO_STATE;
	Animation idle;
	Animation run;
	Animation jump;
	

	uint lastTime = 0u;
	bool jumping = false;
	bool onGround = true;
	X_DIRECTION player_x_dir = NONE;
	bool just_landed = true;


	pugi::xml_attribute starting_x;
	pugi::xml_attribute starting_y;
	pugi::xml_node positionnode;
	pugi::xml_document save_file;

	
	uint jumpfx;
public:


	fPoint player_pos;
	Collider* playerCollider;
	bool touching_floor = false;
	uint currentTime = 0;
	bool ledge_jump_x_disabled = false;
	bool player_died = false;
	bool player_load = false;

	pugi::xml_attribute position_attr_x;
	pugi::xml_attribute position_attr_y;

};




#endif