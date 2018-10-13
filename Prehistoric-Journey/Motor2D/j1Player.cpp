#include "p2Defs.h"
#include "p2Log.h"
#include "j1Player.h"
#include "p2List.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Map.h"

#include "j1Collision.h"
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
	//idle.PushBack({ 118,8,45,57 });
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
	//jump.PushBack({ 215, 219, 56,60 });
	jump.speed = 0.05f;
	jump.loop = false;


	climbing.PushBack({ 21,431,45,61 });
	climbing.PushBack({ 118,431,45,61 });
	climbing.PushBack({ 214,431,45,61 });
	climbing.PushBack({ 312,431,45,61 });
	climbing.speed = 0.05f;//make climbing_idle take the last frame from climbing
	

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
	player_pos.x = App->map->spawn_pos.x;
	player_pos.y = App->map->spawn_pos.y;

	//jumpfx = App->audio->LoadFx("audio/fx/jump.wav");
	
	speed = { 0.0f,0.0f };
	acceleration = { 0.0f, GRAVITY };//change GRAVITY define for value from xml

	collider_offset.x = 5;
	collider_offset.y = 10;
	playerCollider = App->collision->AddCollider({ (int)player_pos.x + collider_offset.x,(int)player_pos.y + collider_offset.y,38,48 }, COLLIDER_PLAYER, this);
	player_rect = { (int)player_pos.x + collider_offset.x,(int)player_pos.y + collider_offset.y,38,48 };

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
	key_w_pressed = false;
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		key_w_pressed = true;
	}

	if (colliding_with_liana == false) {
		
		on_liana = false;
	}
	if (on_liana) {
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			player_pos.x += LIANA_SPEED;
			player_x_dir = RIGHT;
		}
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			player_pos.x -= LIANA_SPEED;
			player_x_dir = LEFT;
		}
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			player_pos.y += LIANA_SPEED;
		}
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		{
			player_pos.y -= LIANA_SPEED;
		}

		state = LIANA;

		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
			jump.Reset();
			state = JUMP;
			speed.y = jump_force_liana;
			
			jumping = true;

			
			on_ground = false;
			just_landed = false;
			on_liana = false;

			if (player_x_dir == LEFT) {
				speed.x = -max_speed_x;
				acceleration.x = -max_acc_x;
			}
			if (player_x_dir == RIGHT) {
				speed.x = max_speed_x;
				acceleration.x = max_acc_x;
			}
		}
		
	}
	else {

		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{

			acceleration.x += ACC_X;
			if (jumping == false) {
				state = RUN;
			}
			else {
				state = JUMP;
			}


			if (player_x_dir == LEFT)
				run.Reset();

			player_x_dir = RIGHT;
			key_d_pressed = true;
		}
		else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			acceleration.x -= ACC_X;
			if (jumping == false) {
				state = RUN;
			}
			else {
				state = JUMP;
			}

			if (player_x_dir == RIGHT) {
				run.Reset();

				player_x_dir = LEFT;
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
		{
			if (jumping == false) {
				state = IDLE;
			}
			acceleration.x = 0.0f;
			speed.x = 0.0f;
		}

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
		{
			if (jumping == false) {
				state = IDLE;
			}
			if (key_d_pressed == false) {
				acceleration.x = 0.0f;
				speed.x = 0.0f;
			}

		}
		key_d_pressed = false;

	}
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && on_ground)
	{
		/*if (playerdir == RIGHT)
			state = JUMP_RIGHT;
		else if (playerdir == LEFT)
			state = JUMP_LEFT;*/
		jump.Reset();
		state = JUMP;
		speed.y = jump_force;
		//acceleration.y = GRAVITY;
		jumping = true;
		
		//player_pos.y -= 5;
		on_ground = false;
		just_landed = false;


		//speed.x = SPEED_X;
		/*if (in_ledge) {
			disable_ledge = SDL_GetTicks();
			ledge_disabled = true;
			ledge_jump_x_disabled = true;
			in_ledge = false;
			on_ledge_left = false;
			on_ledge_right = false;
		}*/
	}



	if (on_ground == false) {
		acceleration.y = GRAVITY;
		just_landed = false;

		/*if (acceleration.x > 0.0f)
			acceleration.x -= FRICTION;
		else if (acceleration.x < 0.0f)
			acceleration.x += FRICTION;

		if (acceleration.x >= -FRICTION && acceleration.x <= FRICTION) {
			acceleration.x = 0.0f;
		}*/
	}
	else// if (playerpos.y >= 400) 
	{

		//acceleration.y = 0.0f;
		on_ground = true;

		//jump_right.Reset();
		//jump_left.Reset();
		//jump.Reset();
		jumping = false;

		if (just_landed == false) {
			
			just_landed = true;

			if (speed.x != 0.0f)
				state = RUN;
			else
				state = IDLE;
		}

		
	}

	if (on_liana)
		acceleration.y = 0.0f;
	
	player_pos.x += speed.x;
	player_pos.y += speed.y;
	speed.x += acceleration.x;
	speed.y += acceleration.y;
	
	colliding_with_liana = false;

	if (speed.x > max_speed_x)
		speed.x = max_speed_x;
	if (acceleration.x > max_acc_x)
		acceleration.x = max_acc_x;

	if (speed.x < -max_speed_x)
		speed.x = -max_speed_x;
	if (acceleration.x < -max_acc_x)
		acceleration.x = -max_acc_x;
	
	//speed.x = 0;
	playerCollider->SetPos(player_pos.x + collider_offset.x, player_pos.y + collider_offset.y);
	//Draw();
	player_rect = { (int)player_pos.x + collider_offset.x, (int)player_pos.y + collider_offset.y, 38, 48 };

	return true;
}



bool j1Player::CleanUp()
{
	LOG("Destroying player");
	bool ret = true;
	return ret;
}


bool j1Player::Load(pugi::xml_node& node)
{
	

	player_pos.x = node.child("position").attribute("x").as_int();
	player_pos.y = node.child("position").attribute("y").as_int();

	return true;
}

bool j1Player::Save(pugi::xml_node& node) const
{
	
	pugi::xml_node position = node.append_child("position");
	position.append_attribute("x").set_value(player_pos.x);
	position.append_attribute("y").set_value(player_pos.y);

	return true;
}


void j1Player::Draw()
{
	switch (state)
	{
	case IDLE:
		current_animation = &idle;
		break;

	case RUN:
		current_animation = &run;
		break;

	case JUMP:
		current_animation = &jump;
		
		break;

	case LIANA:
		current_animation = &climbing;
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
	if (player_x_dir == LEFT && on_liana==false) {
		App->render->Blit(texture, (int)player_pos.x + App->render->camera.x, (int)player_pos.y, &(current_animation->GetCurrentFrame()), NULL, NULL, SDL_FLIP_HORIZONTAL, 0,0);
	}
	else {
		App->render->Blit(texture, player_pos.x, player_pos.y, &(current_animation->GetCurrentFrame()));
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



	if (App->map->data.layers.end != nullptr) {


		MapLayer* layer_coll = App->map->data.layers.end->data;
		iPoint down_right = App->map->WorldToMap(player_rect.x + player_rect.w - collider_offset.x, player_rect.y + player_rect.h);
		iPoint down_left = App->map->WorldToMap(player_rect.x - collider_offset.x, player_rect.y + player_rect.h);

		int down_right_gid = layer_coll->Get(down_right.x, down_right.y);
		int down_left_gid = layer_coll->Get(down_left.x, down_left.y);

		iPoint up_right = App->map->WorldToMap(player_rect.x + player_rect.w - collider_offset.x, player_rect.y);
		iPoint up_left = App->map->WorldToMap(player_rect.x - collider_offset.x, player_rect.y);

		int up_right_gid = layer_coll->Get(up_right.x, up_right.y);
		int up_left_gid = layer_coll->Get(up_left.x, up_left.y);



		if (c2->type == COLLIDER_WALL || c2->type == COLLIDER_LEDGE)
		{
			if (c1->rect.y + c1->rect.h + (int)speed.y + 1 > c2->rect.y && on_ground == false && c1->rect.y < c2->rect.y && (down_right_gid == 48 || down_right_gid == 63 || down_right_gid == 62 || down_right_gid == 348 || down_right_gid == 363 || down_right_gid == 362) && (down_left_gid == 48|| down_left_gid == 63 || down_left_gid == 62|| down_left_gid == 348 || down_left_gid == 363 || down_left_gid == 362))
			{

				acceleration.y = 0.0f;
				speed.y = 0.0f;
				on_ground = true;
			}

			if (c1->rect.y + (int)speed.y - 1 <= c2->rect.y + c2->rect.h && on_ground == false && c1->rect.y + c1->rect.h > c2->rect.y + c2->rect.h && (up_right_gid == 48 || up_right_gid == 63 || up_right_gid == 62|| up_right_gid == 348 || up_right_gid == 363 || up_right_gid == 362) && (up_left_gid == 48 || up_left_gid == 63 || up_left_gid == 62|| up_left_gid == 348 || up_left_gid == 363 || up_left_gid == 362)) {
				if (speed.y < 0.0f) {
					speed.y = -speed.y;
				}
				acceleration.y = GRAVITY;
				player_pos.y = c2->rect.y + c2->rect.h + 1;

			}
			else
			{
				if (c1->rect.x + c1->rect.w + (int)speed.x + 1 > c2->rect.x  && c1->rect.y + c1->rect.h - 15 > c2->rect.y && player_x_dir == RIGHT && abs(c1->rect.x) < abs(c2->rect.x)) {

					acceleration.x = 0.0f;
					speed.x = 0.0f;
					player_pos.x--;

				}
				else if (c1->rect.x + (int)speed.x - 1 < c2->rect.x + c2->rect.w && c1->rect.y + c1->rect.h - 15 > c2->rect.y && player_x_dir == LEFT && c1->rect.x > c2->rect.x) {

					acceleration.x = 0.0f;
					speed.x = 0.0f;
					player_pos.x++;

				}
			}

			if (c2->type == COLLIDER_LEDGE && c1->rect.x + (int)speed.x + 1 >= c2->rect.x + c2->rect.w) {
				on_ground = false;
			}
			else	if (c2->type == COLLIDER_LEDGE && c1->rect.x + c1->rect.w + (int)speed.x - 1 <= c2->rect.x) {
				on_ground = false;
			}
		}

		//****LIANAS
		if (c2->type == COLLIDER_LIANA) {
			colliding_with_liana = true;
		}


		if (c2->type == COLLIDER_LIANA && key_w_pressed) {
			acceleration = { 0.0f,0.0f };
			speed = { 0.0f,0.0f };
			on_liana = true;
		}

	}
		/*if (c1->rect.y < c2->rect.y + c2->rect.h && c1->rect.y + 3 > c2->rect.y + c2->rect.h)

		{
			player_pos.y = player_pos.y + 1;
			speed.y = GRAVITY;



		}
		else if (c1->rect.y + c1->rect.h > c2->rect.y && c1->rect.y + c1->rect.h - 3< c2->rect.y)

		{

			touching_floor = true;
			if ((c1->rect.x + 5 >= c2->rect.x + c2->rect.w && c1->rect.x - 5 <= c2->rect.x + c2->rect.w)
				|| (c1->rect.x + c1->rect.w - 5 <= c2->rect.x && c1->rect.x + c1->rect.w + 5 >= c2->rect.x)) {
				touching_floor = false;

			}

		}


		else if (c1->rect.x + c1->rect.w > c2->rect.x && c1->rect.x + c1->rect.w - 3 < c2->rect.x)
		{
			player_pos.x = player_pos.x - 1;

		}
		else if (c1->rect.x < c2->rect.x + c2->rect.w && c1->rect.x + 3 > c2->rect.x + c2->rect.w)
		{
			player_pos.x = player_pos.x + 1;

		}

	}*/





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