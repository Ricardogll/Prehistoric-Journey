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
#define ACC_X 0.25f
#define FRICTION 0.25f
#define LIANA_SPEED 1.0f

struct SDL_texture;
struct SDL_Rect;
struct Collider;
struct Mix_Chunk;


enum STATE {
	IDLE,
	RUN,
	JUMP,
	LIANA,
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
	
	void OnCollision(Collider* c1, Collider* c2);
private:

	SDL_Rect player_rect;

	/*uint width = 0u;
	uint height = 0u;*/
	SDL_Texture* texture = nullptr;
	fPoint speed;
	fPoint acceleration;
	float max_acc_x = 0.5f;
	float max_speed_x = 2.0f;
	float jump_force = -5.0f;
	float jump_force_liana = -4.0f;
	STATE state = NO_STATE;
	Animation* current_animation = nullptr;

	//STATE last_state = NO_STATE;
	//STATE last_state_2 = NO_STATE;
	Animation idle;
	Animation run;
	Animation jump;
	Animation climbing;
	Animation climbing_idle;

	//uint lastTime = 0u;
	bool jumping = false;
	bool on_ground = false;
	bool on_liana = false;
	X_DIRECTION player_x_dir = NONE;
	bool just_landed = true;
	bool key_d_pressed = false;
	bool key_w_pressed = false;
	bool colliding_with_liana = false;
	//int collision_extra = 3;

	pugi::xml_attribute starting_x;
	pugi::xml_attribute starting_y;
	pugi::xml_node positionnode;
	pugi::xml_document save_file;

	
	uint jumpfx;
public:


	fPoint player_pos;
	Collider* playerCollider;
	iPoint collider_offset;
	//bool touching_floor = false;
	//uint currentTime = 0;
	//bool ledge_jump_x_disabled = false;
	bool player_died = false;
	//bool player_load = false;


};




#endif