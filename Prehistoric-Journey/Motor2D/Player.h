#ifndef _PLAYER_H_
#define _PLAYER_H_

#include	"PugiXml/src/pugixml.hpp"
#include	"j1Module.h"
#include	"p2List.h"
#include	"p2Point.h"
#include	"j1Textures.h"
#include	"Animation.h"
#include	"Entity.h"




struct SDL_texture;
struct SDL_Rect;  // dont need this probably, check and delete
struct Collider;
struct Mix_Chunk; // dont need this probably, check and delete



enum STATE {
	IDLE,
	RUN,
	JUMP,
	LIANA,
	LIANA_IDLE,
	ATTACK,
	NO_STATE,
};

enum X_DIRECTION {
	RIGHT,
	LEFT,
	NONE,
};


class Player :public Entity
{
public:
	Player(int x, int y, EntityTypes type);
	~Player();

	bool Start();
	bool CleanUp();
	bool Awake(pugi::xml_node&);
	void Update(float dt);
	bool PostUpdate();
	bool Save(pugi::xml_node&) const;
	bool Load(pugi::xml_node&);

	

	void Draw();
	
	void OnCollision( Collider* c1,  Collider* c2);

	void LoadVariablesXML(const pugi::xml_node& player_node);

	void SetAnimations(pugi::xml_node& config, Animation& animation);
	
	void AnimationsApplyDt();

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
	bool attacking = false;
	
	SDL_Texture* texture = nullptr;

	Animation* current_animation = nullptr;
	Animation idle = Animation();
	Animation run = Animation();
	Animation jump = Animation();
	Animation climbing = Animation();
	Animation climbing_idle = Animation();
	Animation attack = Animation();
	
	bool anim_speed_flag = false;
	float idle_anim_speed = 0.0f;
	float run_anim_speed = 0.0f;
	float jump_anim_speed = 0.0f;
	float climbing_anim_speed = 0.0f;
	//float climbing_idle_anim_speed = 0.0f;
	float attack_anim_speed = 0.0f;
	uint32 start_attack = 0;

	Collider* player_collider = nullptr;

	uint jump_fx = 0u;
	uint lose_fx = 0u;

	float dt_current = 0.0f;

	//****XML variables

	float gravity = 0.0f;
	float acceleration_x = 0.0f;
	float liana_speed = 0.0f;
	float max_acc_x = 0.0f;
	float max_speed_x = 0.0f;
	float jump_force = 0.0f;
	float jump_force_liana = 0.0f;
	iPoint collider_offset = { 0,0 };
	iPoint collider_dimensions = { 0,0 };
	p2SString player_spritesheet;
	p2SString jump_fx_folder;
	p2SString lose_fx_folder;

public:

	fPoint last_saved_pos = { 0.0f,0.0f };
	fPoint player_pos = { 0.0f,0.0f };
	int saved_map = 1;
	bool player_died = false;
	bool change_map = false;
	bool god_mode = false;
	int limit_map = 0;
	int start_map = 0;
	int attack_time = 0;

};
#endif