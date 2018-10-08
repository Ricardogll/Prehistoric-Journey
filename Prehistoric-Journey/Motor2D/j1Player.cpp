#include "p2Defs.h"
#include "p2Log.h"
#include "j1Player.h"
#include "p2List.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Map.h"

//#include "j1Collision.h"
#include "j1Audio.h"
#include "j1Window.h"




j1Player::j1Player() : j1Module()
{

	name.create("player");

	idle.PushBack({ 21,8,45,57 });
	idle.PushBack({ 118,8,45,57 });
	idle.PushBack({ 215,8,45,57 });
	idle.PushBack({ 312,8,45,57 });
	idle.PushBack({ 409,8,45,57 });
	idle.PushBack({ 506,8,45,57 });
	idle.PushBack({ 118,8,45,57 });
	idle.speed = 0.05f;
	
	
	/*run.PushBack({ 112,76,60,61 });
	run.PushBack({ 209,76,60,61 });
	run.PushBack({ 306,76,60,61 });
	run.PushBack({ 403,76,60,61 });
	run.PushBack({ 500,76,60,61 });
	run.PushBack({ 15,148,60,61 });
	run.PushBack({ 112,148,60,61 });
	run.PushBack({ 209,148,60,61 });
	run.PushBack({ 306,148,60,61 });
	run.PushBack({ 403,148,60,61 });
	run.PushBack({ 500,148,60,61 });
	run.PushBack({ 15,148,60,61 });
	run.PushBack({ 112,219,60,61 });
	run.PushBack({ 212,219,60,61 });*/
	run.PushBack({ 112,76,60,61 });
	run.PushBack({ 209,76,60,61 });
	run.PushBack({ 306,76,60,61 });
	run.PushBack({ 112,76,60,61 });
	run.PushBack({ 403,76,60,61 });
	run.PushBack({ 500,76,60,61 });
	run.PushBack({ 15,148,60,61 });
	run.PushBack({ 112,148,60,61 });
	run.PushBack({ 500,76,60,61 });
	run.PushBack({ 403,76,60,61 });
	run.speed = 0.08f;


	jump.PushBack({ 211, 148, 56,60 });
	jump.PushBack({ 405, 148, 56,60 });
	jump.PushBack({ 502, 148, 56,60 });
	jump.PushBack({ 17, 219, 56,60 });
	jump.PushBack({ 114, 219, 56,60 });
	jump.PushBack({ 215, 219, 56,60 });
	jump.speed = 0.05f;
	jump.loop = false;
}




j1Player::~j1Player()
{

}


bool j1Player::Awake(pugi::xml_node& config)
{
	LOG("Loading Player");
	bool ret = true;

	/*save_file.load_file("save_game.xml");
	positionnode = save_file.child("game_state").child("position");
	position_attr_x = positionnode.child("position").attribute("x");
	position_attr_y = positionnode.child("position").attribute("y");
	starting_x = positionnode.child("start").attribute("x");
	starting_y = positionnode.child("start").attribute("y");*/





	return ret;
}

bool j1Player::Start()
{


	LOG("starting player");
	bool ret = true;

	
	texture = App->tex->Load("textures/caverman.png");
	state = IDLE;
	playerpos.x = 200;
	playerpos.y = 200;//Get from tiled later

	//jumpfx = App->audio->LoadFx("audio/fx/jump.wav");

	

	
	speed.x = 0;

	speed.y = 0;
	//playerCollider = App->collision->AddCollider({ (int)playerpos.x,(int)playerpos.y,20,35 }, COLLIDER_PLAYER, this);

	return ret;
}

bool j1Player::Update(float dt)
{
	

	return true;
}

bool j1Player::PostUpdate()
{
	//currentTime = SDL_GetTicks();

	//if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) {
	//	SavePosition();
	//}

	//if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) {
	//	LoadPosition();
	//	player_load = true;
	//	state = IDLE_RIGHT;
	//	touching_floor = false;
	//	speed.y = 0;
	//	speed.x = 0;

	//}

	//if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {
	//	playerpos.x = starting_x.as_float();
	//	playerpos.y = starting_y.as_float();
	//	player_load = true;
	//	state = IDLE_RIGHT;
	//	touching_floor = false;
	//	speed.y = 0;
	//	speed.x = 0;
	//}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT )
	{

		speed.x = SPEED_X;
		if (jumping == false) {
			state = WALK;
		}
		else {
			state = JUMP;
		}


		if (player_x_dir == LEFT)
			run.Reset();

		player_x_dir = RIGHT;
	}
	/*if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
	{
		if (jumping == false) {
			state = IDLE_RIGHT;
		}
	}*/

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT )
	{
		speed.x = -SPEED_X;
		if (jumping == false) {
			state = WALK;
		}
		else {
			state = JUMP;
		}

		if (player_x_dir == RIGHT)
			run.Reset();

		player_x_dir = LEFT;
	}
	//if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP )
	//{
	//	if (jumping == false) {
	//		state = IDLE_LEFT;
	//	}

	//}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		/*if (playerdir == RIGHT)
			state = JUMP_RIGHT;
		else if (playerdir == LEFT)
			state = JUMP_LEFT;*/
		state = JUMP;
		Jumping();
		speed.x = SPEED_X;
		/*if (in_ledge) {
			disable_ledge = SDL_GetTicks();
			ledge_disabled = true;
			ledge_jump_x_disabled = true;
			in_ledge = false;
			on_ledge_left = false;
			on_ledge_right = false;
		}*/
	}



	if (touching_floor == false && playerpos.y < 300) { //change when collisions are implemented
		speed.y += GRAVITY;

	}
	else// if (playerpos.y >= 400) 
	{

		speed.y = 0.0f;
		onGround = true;

		//jump_right.Reset();
		//jump_left.Reset();
		//jump.Reset();
		jumping = false;
		
	}



	//if (on_ledge_right) {

	//	state = LEDGE_RIGHT;
	//}

	//if (on_ledge_left) {

	//	state = LEDGE_LEFT;
	//}

	//if (currentTime > disable_ledge + 50 && disable_ledge != 0) {
	//	ledge_jump_x_disabled = false;
	//	ledge_disabled = false;
	//	LOG("jump x enabled & ledge ON");
	//	disable_ledge = 0;

	//}
	playerpos.x += speed.x;
	playerpos.y += speed.y;
	speed.x = 0;
	//playerCollider->SetPos(playerpos.x, playerpos.y);
	//Draw();

	

	return true;
}

bool j1Player::Jumping() {
	bool ret = true;

	if (onGround)
	{
		jump.Reset();
		
		jumping = true;
		speed.y = -5.0f;
		onGround = false;
		playerpos.y -= 5;
		touching_floor = false;
		App->audio->PlayFx(jumpfx);
	}

	if (speed.y < -3.2f) {
		speed.y = -3.2f;
	}

	return ret;
}


bool j1Player::CleanUp()
{
	LOG("Destroying player");
	bool ret = true;
	return ret;
}


bool j1Player::Load(pugi::xml_node& node)
{
	bool ret = true;



	return ret;
}




void j1Player::Draw()
{
	switch (state)
	{
	case IDLE:
		current_animation = &idle;
		break;

	case WALK:
		current_animation = &run;
		break;

	case JUMP:
		current_animation = &jump;
		if (current_animation->Finished()) {
			jump.Reset();
			state = NO_STATE;
		}
		break;

	default:
		current_animation = &idle;

	}


	////so jump animation doesnt get stuck if we just jump and not press anything else after
	//if (current_animation->Finished() && current_animation == &jump_right && touching_floor) {
	//	current_animation = &idle_right;
	//}
	//else if (current_animation->Finished() && current_animation == &jump_left && touching_floor) {
	//	current_animation = &idle_left;
	//}

	/*if (current_animation==&jump && onGround)
	{
		
		current_animation = &idle;
	}*/
	SDL_Rect r = current_animation->GetCurrentFrame();
	if (player_x_dir == LEFT) {
		App->render->Blit(texture, playerpos.x, playerpos.y, &(current_animation->GetCurrentFrame()), NULL, NULL, SDL_FLIP_HORIZONTAL);
	}
	else {
		App->render->Blit(texture, playerpos.x, playerpos.y, &(current_animation->GetCurrentFrame()));
	}

	//if (last_state != last_state_2)
	//	last_state_2 = last_state;

	//if (last_state != state)
	//	last_state = state;
	


}




void j1Player::LoadPosition() {


	//playerpos.x = position_attr_x.as_int();
	//playerpos.y = position_attr_y.as_int();

}


void j1Player::SavePosition() {
	//position_attr_x.set_value(playerpos.x);
	//position_attr_y.set_value(playerpos.y);
}


void j1Player::OnCollision(Collider* c1, Collider* c2) {

	//if (c2->type == COLLIDER_FLOOR)
	//{


	//	if (c1->rect.y < c2->rect.y + c2->rect.h && c1->rect.y + 3 > c2->rect.y + c2->rect.h)

	//	{
	//		playerpos.y = playerpos.y + 1;
	//		speed.y = GRAVITY;



	//	}
	//	else if (c1->rect.y + c1->rect.h > c2->rect.y && c1->rect.y + c1->rect.h - 3< c2->rect.y)

	//	{

	//		touching_floor = true;
	//		if ((c1->rect.x + 5 >= c2->rect.x + c2->rect.w && c1->rect.x - 5 <= c2->rect.x + c2->rect.w)
	//			|| (c1->rect.x + c1->rect.w - 5 <= c2->rect.x && c1->rect.x + c1->rect.w + 5 >= c2->rect.x)) {
	//			touching_floor = false;

	//		}

	//	}


	//	else if (c1->rect.x + c1->rect.w > c2->rect.x && c1->rect.x + c1->rect.w - 3 < c2->rect.x)
	//	{
	//		playerpos.x = playerpos.x - 1;

	//	}
	//	else if (c1->rect.x < c2->rect.x + c2->rect.w && c1->rect.x + 3 > c2->rect.x + c2->rect.w)
	//	{
	//		playerpos.x = playerpos.x + 1;

	//	}



	//}
	//if (c2->type == COLLIDER_DIE)
	//{
	//	playerpos.x = starting_x.as_float();
	//	playerpos.y = starting_y.as_float();
	//	speed.x = 0;
	//	speed.y = 0;
	//	player_died = true;

	//}

	//if (c2->type == COLLIDER_LEDGE && in_ledge == false && ledge_disabled == false)
	//{


	//	//same as FLOOR until...
	//	if (c1->rect.y < c2->rect.y + c2->rect.h && c1->rect.y + 3 > c2->rect.y + c2->rect.h)

	//	{
	//		playerpos.y = playerpos.y + 1;
	//		speed.y = GRAVITY;



	//	}
	//	else if (c1->rect.y + c1->rect.h > c2->rect.y && c1->rect.y + c1->rect.h - 3< c2->rect.y)

	//	{


	//		touching_floor = true;
	//		if ((c1->rect.x + 5 >= c2->rect.x + c2->rect.w && c1->rect.x - 5 <= c2->rect.x + c2->rect.w)
	//			|| (c1->rect.x + c1->rect.w - 5 <= c2->rect.x && c1->rect.x + c1->rect.w + 5 >= c2->rect.x)) {
	//			touching_floor = false;

	//		}

	//	}


	//	else if (c1->rect.x + c1->rect.w > c2->rect.x && c1->rect.x + c1->rect.w - 3 < c2->rect.x)
	//	{
	//		playerpos.x = playerpos.x - 1;

	//	}
	//	else if (c1->rect.x < c2->rect.x + c2->rect.w && c1->rect.x + 3 > c2->rect.x + c2->rect.w)
	//	{
	//		playerpos.x = playerpos.x + 1;

	//	}


	//	// here.
	//	//if player y is between ledge y and half of its height, then he will grab


	//	if (c2->rect.y < c1->rect.y && c1->rect.y - (c2->rect.h / 2) < c2->rect.y) {
	//		speed.x = 0;
	//		ledge_right.Reset();
	//		ledge_left.Reset();

	//		if (c1->rect.x < c2->rect.x) {
	//			state = LEDGE_RIGHT;
	//			on_ledge_right = true;
	//		}
	//		else {
	//			state = LEDGE_LEFT;
	//			on_ledge_left = true;
	//		}
	//		in_ledge = true;
	//	}


	//}
}