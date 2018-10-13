#ifndef _j1PLAYER_H_
#define _j1PLAYER_H_

#include	"PugiXml/src/pugixml.hpp"
#include	"j1Module.h"
#include	"p2List.h"
#include	"p2Point.h"
#include	"j1Textures.h"
#include	"Animation.h"




struct SDL_texture;
struct SDL_Rect;
struct Collider;
struct Mix_Chunk;


enum STATE {
	IDLE,
	RUN,
	JUMP,
	LIANA,
	LIANA_IDLE,
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
	

	void Draw();
	
	void OnCollision(Collider* c1, Collider* c2);

	void LoadVariablesXML(pugi::xml_node& player_node);
private:

	SDL_Rect player_rect = { 0,0 };
	fPoint speed = { 0.0f,0.0f };
	fPoint acceleration{ 0.0f,0.0f };
	STATE state = NO_STATE;
	
	bool jumping = false;
	bool on_ground = false;
	bool on_liana = false;
	X_DIRECTION player_x_dir = NONE;
	bool just_landed = true;
	bool key_d_pressed = false;
	bool key_w_pressed = false;
	bool colliding_with_liana = false;
	
	SDL_Texture* texture = nullptr;

	Animation* current_animation = nullptr;
	Animation idle = Animation();
	Animation run = Animation();
	Animation jump = Animation();
	Animation climbing = Animation();
	Animation climbing_idle = Animation();
	
	Collider* player_collider = nullptr;

	uint jumpfx;

	//****XML variables

	float GRAVITY = 0.0f;
	float SPEED_X = 0.0f;
	float SPEED_Y = 0.0f;
	float ACCELERATION_X = 0.0f;
	float FRICTION = 0.0f;
	float LIANA_SPEED = 0.0f;
	float MAX_ACC_X = 0.0f;
	float MAX_SPEED_X = 0.0f;
	float JUMP_FORCE = 0.0f;
	float JUMP_FORCE_LIANA = 0.0f;

public:


	fPoint player_pos = { 0.0f,0.0f };
	
	iPoint collider_offset = { 0,0 };
	bool player_died = false;
	


};




#endif