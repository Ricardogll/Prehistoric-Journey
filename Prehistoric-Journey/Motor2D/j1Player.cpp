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
#include "j1FadeToBlack.h"



j1Player::j1Player() : j1Module()
{

	name.create("player");

	idle.PushBack({ 21,8,45,57 });
	idle.PushBack({ 118,8,45,57 });
	idle.PushBack({ 215,8,45,57 });
	idle.PushBack({ 312,8,45,57 });
	idle.PushBack({ 409,8,45,57 });
	idle.PushBack({ 506,8,45,57 });
	
	idle.speed = 0.05f;
	
	

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
	
	jump.speed = 0.05f;
	jump.loop = false;


	climbing.PushBack({ 21,431,45,61 });
	climbing.PushBack({ 118,431,45,61 });
	climbing.PushBack({ 214,431,45,61 });
	climbing.PushBack({ 312,431,45,61 });
	climbing.speed = 0.05f;

	climbing_idle.PushBack({ 21,431,45,61 });
	climbing_idle.speed = 0.0f;
	

}




j1Player::~j1Player()
{

}


bool j1Player::Awake(pugi::xml_node& config)
{
	LOG("Loading Player");
	bool ret = true;

	if (config != NULL)
		LoadVariablesXML(config);




	return ret;
}

bool j1Player::Start()
{


	LOG("starting player");
	bool ret = true;

	jump_fx = App->audio->LoadFx("audio/fx/jump.wav");
	lose_fx = App->audio->LoadFx("audio/fx/lose.wav");
	texture = App->tex->Load("textures/caverman.png");
	state = IDLE;
	player_pos.x = App->map->spawn_pos.x;
	player_pos.y = App->map->spawn_pos.y;
	player_x_dir = RIGHT;

	//jumpfx = App->audio->LoadFx("audio/fx/jump.wav");
	
	speed = { 0.0f,0.0f };
	acceleration = { 0.0f, GRAVITY };

	collider_offset.x = 5;
	collider_offset.y = 10;
	player_collider = App->collision->AddCollider({ (int)player_pos.x + collider_offset.x,(int)player_pos.y + collider_offset.y,38,48 }, COLLIDER_PLAYER, this);
	player_rect = { (int)player_pos.x + collider_offset.x,(int)player_pos.y + collider_offset.y,38,48 };

	return ret;
}

bool j1Player::Update(float dt)
{
	

	return true;
}

bool j1Player::PostUpdate()
{
	App->render->camera.x = -player_pos.x - player_rect.w/2 + App->render->camera.w / 2;
	if (App->render->camera.x > 0)
		App->render->camera.x = 0;
	if (App->render->camera.x < -5375)
		App->render->camera.x = -5375;

	key_w_pressed = false;
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		key_w_pressed = true;
	}

	if (colliding_with_liana == false) {
		
		on_liana = false;
	}
	if (on_liana) {
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
		{
			state = LIANA_IDLE;
		}else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
		{
			state = LIANA_IDLE;
		}else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_UP)
		{
			state = LIANA_IDLE;
		}else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_UP)
		{
			state = LIANA_IDLE;
		}


		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			player_pos.x += LIANA_SPEED;
			player_x_dir = RIGHT;
			state = LIANA;
		}
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			player_pos.x -= LIANA_SPEED;
			player_x_dir = LEFT;
			state = LIANA;
		}
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			player_pos.y += LIANA_SPEED;
			state = LIANA;
		}
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		{
			player_pos.y -= LIANA_SPEED;
			state = LIANA;
		}

		

		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
			jump.Reset();
			state = JUMP;
			speed.y = JUMP_FORCE_LIANA;
			
			jumping = true;

			
			on_ground = false;
			just_landed = false;
			on_liana = false;

			if (player_x_dir == LEFT) {
				speed.x = -MAX_SPEED_X;
				acceleration.x = -MAX_ACC_X;
			}
			if (player_x_dir == RIGHT) {
				speed.x = MAX_SPEED_X;
				acceleration.x = MAX_ACC_X;
			}

			App->audio->PlayFx(jump_fx);
		}
		
	}
	else {

		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{

			acceleration.x += ACCELERATION_X;
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
			acceleration.x -= ACCELERATION_X;
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
		
		jump.Reset();
		state = JUMP;
		speed.y = JUMP_FORCE;
		
		jumping = true;
		
		
		on_ground = false;
		just_landed = false;

		App->audio->PlayFx(jump_fx);
	
	}



	if (on_ground == false) {
		acceleration.y = GRAVITY;
		just_landed = false;

	
	}
	else 
	{

		
		on_ground = true;

		
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

	if (speed.x > MAX_SPEED_X)
		speed.x = MAX_SPEED_X;
	if (acceleration.x > MAX_ACC_X)
		acceleration.x = MAX_ACC_X;

	if (speed.x < -MAX_SPEED_X)
		speed.x = -MAX_SPEED_X;
	if (acceleration.x < -MAX_ACC_X)
		acceleration.x = -MAX_ACC_X;
	
	
	player_collider->SetPos(player_pos.x + collider_offset.x, player_pos.y + collider_offset.y);
	
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
	

	last_saved_pos.x = node.child("position").attribute("x").as_int();
	last_saved_pos.y = node.child("position").attribute("y").as_int();
	player_pos.x = last_saved_pos.x;
	player_pos.y = last_saved_pos.y;

	return true;
}

bool j1Player::Save(pugi::xml_node& node) const
{
	
	pugi::xml_node position = node.append_child("position");
	position.append_attribute("x").set_value(player_pos.x);
	position.append_attribute("y").set_value(player_pos.y);
	
	saved_map = CurrentMap::map_1;

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
	case LIANA_IDLE:
		current_animation = &climbing_idle;
		break;
	default:
		current_animation = &idle;

	}



	SDL_Rect r = current_animation->GetCurrentFrame();
	if (player_x_dir == LEFT && on_liana==false) {
		App->render->Blit(texture, (int)player_pos.x + App->render->camera.x, (int)player_pos.y, &(current_animation->GetCurrentFrame()), NULL, NULL, SDL_FLIP_HORIZONTAL, 0,0);
	}
	else {
		App->render->Blit(texture, player_pos.x, player_pos.y, &(current_animation->GetCurrentFrame()));
	}

	
	


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
			{ //Using "(int)speed" to see if in the next update player will be inside the wall. Using +1 in case the float is shortened and we end up going inside the wall.
				if (c1->rect.y + c1->rect.h + (int)speed.y + 1 > c2->rect.y && on_ground == false && c1->rect.y < c2->rect.y && (down_right_gid == 48 || down_right_gid == 63 || down_right_gid == 62 || down_right_gid == 198 || down_right_gid == 213 || down_right_gid == 212) && (down_left_gid == 48 || down_left_gid == 63 || down_left_gid == 62 || down_left_gid == 198 || down_left_gid == 213 || down_left_gid == 212)) {

					acceleration.y = 0.0f;
					speed.y = 0.0f;
					on_ground = true;
				}

				if (c1->rect.y + (int)speed.y - 1 <= c2->rect.y + c2->rect.h && on_ground == false && c1->rect.y + c1->rect.h > c2->rect.y + c2->rect.h && (up_right_gid == 48 || up_right_gid == 63 || up_right_gid == 62 || up_right_gid == 198 || up_right_gid == 213 || up_right_gid == 212) && (up_left_gid == 48 || up_left_gid == 63 || up_left_gid == 62 || up_left_gid == 198 || up_left_gid == 213 || up_left_gid == 212)) {
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

			//****** LEVEL END

			if (c2->type == COLLIDER_LEVEL_END && change_map == false) {
				change_map = true;
				
			}

			if (c2->type == COLLIDER_DEAD && player_died == false) {
				player_died = true;
				App->audio->PlayFx(lose_fx);
				player_pos = last_saved_pos;
				
			}
		}
	
}


void j1Player::LoadVariablesXML(pugi::xml_node& player_node) {

	pugi::xml_node variables = player_node.child("variables");

	GRAVITY = variables.child("GRAVITY").attribute("value").as_float();
	SPEED_X = variables.child("SPEED_X").attribute("value").as_float();
	SPEED_Y = variables.child("SPEED_Y").attribute("value").as_float();
	ACCELERATION_X = variables.child("ACCELERATION_X").attribute("value").as_float();
	FRICTION = variables.child("FRICTION").attribute("value").as_float();
	LIANA_SPEED = variables.child("LIANA_SPEED").attribute("value").as_float();
	MAX_ACC_X = variables.child("MAX_ACC_X").attribute("value").as_float();
	MAX_SPEED_X = variables.child("MAX_SPEED_X").attribute("value").as_float();
	JUMP_FORCE = variables.child("JUMP_FORCE").attribute("value").as_float();
	JUMP_FORCE_LIANA = variables.child("JUMP_FORCE_LIANA").attribute("value").as_float();
}