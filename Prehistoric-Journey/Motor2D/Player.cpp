#include "p2Defs.h"
#include "p2Log.h"
#include "Player.h"
#include "p2List.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Map.h"

#include "j1Collision.h"
#include "j1Audio.h"
#include "j1Window.h"
#include "j1FadeToBlack.h"



Player::Player(int x, int y, pugi::xml_node& config, EntityTypes type) : Entity(x, y, type)
{
	pugi::xml_node node_player = config.child("player");

	Awake(node_player);
	Start();

}

Player::~Player()
{

}

bool Player::Awake(pugi::xml_node& config)
{
	LOG("Loading Player");
	bool ret = true;

	if (config != NULL) {
		LoadVariablesXML(config);

		pugi::xml_node animations = config.child("animations");
		SetAnimations(animations.child("idle").child("animation"), idle);
		idle.speed = animations.child("idle").attribute("speed").as_float();
		idle.loop = animations.child("idle").attribute("loop").as_bool();

		SetAnimations(animations.child("run").child("animation"), run);
		run.speed = animations.child("run").attribute("speed").as_float();
		run.loop = animations.child("run").attribute("loop").as_bool();

		SetAnimations(animations.child("jump").child("animation"), jump);
		jump.speed = animations.child("jump").attribute("speed").as_float();
		jump.loop = animations.child("jump").attribute("loop").as_bool();

		SetAnimations(animations.child("climb").child("animation"), climbing);
		climbing.speed = animations.child("climb").attribute("speed").as_float();
		climbing.loop = animations.child("climb").attribute("loop").as_bool();

		SetAnimations(animations.child("climbidle").child("animation"), climbing_idle);
		climbing_idle.speed = animations.child("climbidle").attribute("speed").as_float();
		climbing_idle.loop = animations.child("climbidle").attribute("loop").as_bool();

		SetAnimations(animations.child("attack").child("animation"), attack);
		attack.speed = animations.child("attack").attribute("speed").as_float();
		attack.loop = animations.child("attack").attribute("loop").as_bool();
	}

	return ret;
}

bool Player::Start()
{


	LOG("starting player");
	bool ret = true;

	jump_fx = App->audio->LoadFx(jump_fx_folder.GetString());
	lose_fx = App->audio->LoadFx(lose_fx_folder.GetString());
	texture = App->tex->Load(spritesheet.GetString());
	state = IDLE;
	player_pos.x = App->map->spawn_pos.x;
	player_pos.y = App->map->spawn_pos.y;
	entity_x_dir = RIGHT;

	last_saved_pos.x = App->map->spawn_pos.x;
	last_saved_pos.y = App->map->spawn_pos.y;

	speed = { 0.0f,0.0f };
	acceleration = { 0.0f, 0.0f };

	
	collider = App->collision->AddCollider({ (int)player_pos.x + collider_offset.x,(int)player_pos.y + collider_offset.y,collider_dimensions.x,collider_dimensions.y }, COLLIDER_PLAYER, (j1Module*)App->entities);
	player_rect = { (int)player_pos.x + collider_offset.x,(int)player_pos.y + collider_offset.y,collider_dimensions.x,collider_dimensions.y };

	return ret;
}

void Player::Update(float dt)
{
	dt_current = dt;


	App->render->camera.x = -player_pos.x - player_rect.w / 2 + App->win->width / 2;
	if (App->render->camera.x > start_map)
		App->render->camera.x = start_map;
	if (App->render->camera.x < limit_map)
		App->render->camera.x = limit_map;


	AnimationsApplyDt();



	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		god_mode = !god_mode;

	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {
		if (App->map->debug_camera_culling == 0)
			App->map->debug_camera_culling = 1;
		else
			App->map->debug_camera_culling = 0;
	}


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
		}
		else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
		{
			state = LIANA_IDLE;
		}
		else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_UP)
		{
			state = LIANA_IDLE;
		}
		else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_UP)
		{
			state = LIANA_IDLE;
		}


		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			player_pos.x += liana_speed * dt_current;
			entity_x_dir = RIGHT;
			state = LIANA;
		}
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			player_pos.x -= liana_speed * dt_current;
			entity_x_dir = LEFT;
			state = LIANA;
		}
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			player_pos.y += liana_speed * dt_current;
			state = LIANA;
		}
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		{
			player_pos.y -= liana_speed * dt_current;
			state = LIANA;
		}



		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && god_mode == false) {
			jump.Reset();
			state = JUMP;
			speed.y = jump_force_liana;

			jumping = true;


			on_ground = false;
			just_landed = false;
			on_liana = false;

			if (entity_x_dir == LEFT) {
				speed.x = -max_speed_x;
				acceleration.x = -max_acc_x;
			}
			if (entity_x_dir == RIGHT) {
				speed.x = max_speed_x;
				acceleration.x = max_acc_x;
			}

			App->audio->PlayFx(jump_fx);
		}

	}
	else {

		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && attacking == false)
		{

			acceleration.x += acceleration_x;
			if (jumping == false) {
				state = RUN;
			}
			else {
				state = JUMP;
			}


			if (entity_x_dir == LEFT)
				run.Reset();

			entity_x_dir = RIGHT;
			key_d_pressed = true;
		}
		else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && attacking == false)
		{
			acceleration.x -= acceleration_x;
			if (jumping == false) {
				state = RUN;
			}
			else {
				state = JUMP;
			}

			if (entity_x_dir == RIGHT) {
				run.Reset();

				entity_x_dir = LEFT;
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
			//if (key_d_pressed == false) { 
			acceleration.x = 0.0f;
			speed.x = 0.0f;
			//}

		}

		if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN && attacking == false && on_ground == true)
		{
			attack.Reset();
			start_attack = SDL_GetTicks();
			attacking = true;
		}

		if (start_attack + attack_time < SDL_GetTicks() && attacking == true)
		{
			state = IDLE;
			start_attack = 0;
			attacking = false;
		}

		if (attacking == true)
		{
			state = ATTACK;
			acceleration.x = 0.0f;
			speed.x = 0.0f;
		}

		key_d_pressed = false;

	}


	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && on_ground && !god_mode)
	{

		jump.Reset();
		state = JUMP;
		speed.y = jump_force;

		jumping = true;


		on_ground = false;
		just_landed = false;

		App->audio->PlayFx(jump_fx);
	}



	if (on_ground == false) {
		acceleration.y = gravity;
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

	if (god_mode == true) //CHANGE THIS NUMBERS TO XML AND PUT DT
	{
		acceleration.y = 0.0f;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			acceleration.x += acceleration_x;
		}
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			acceleration.x -= acceleration_x;
		}
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{

			speed.y = acceleration_x;
		}

		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_UP)
		{
			speed.y = 0.0f;
		}
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		{
			speed.y = -acceleration_x;
		}
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_UP)
		{
			speed.y = 0.0f;
		}
	}

	if (on_liana)
		acceleration.y = 0.0f;

	player_pos.x += speed.x * dt_current;
	player_pos.y += speed.y * dt_current;
	speed.x += acceleration.x * dt_current;
	speed.y += acceleration.y * dt_current;

	colliding_with_liana = false;

	if (speed.x > max_speed_x)
		speed.x = max_speed_x;

	if (acceleration.x > max_acc_x)
		acceleration.x = max_acc_x;


	if (speed.x < -max_speed_x)
		speed.x = -max_speed_x;

	if (acceleration.x < -max_acc_x)
		acceleration.x = -max_acc_x;




	collider->SetPos(player_pos.x + collider_offset.x, player_pos.y + collider_offset.y);

	player_rect = { (int)player_pos.x + collider_offset.x, (int)player_pos.y + collider_offset.y, 38, 48 };




	collider->SetPos(player_pos.x + collider_offset.x, player_pos.y + collider_offset.y);

	player_rect = { (int)player_pos.x + collider_offset.x, (int)player_pos.y + collider_offset.y, collider_dimensions.x, collider_dimensions.y };



	
}



bool Player::PostUpdate()
{
	
	return true;
}

bool Player::CleanUp()
{
	LOG("Destroying player");
	bool ret = true;

	

	return ret;
}


bool Player::Load(pugi::xml_node& node)
{
	

	last_saved_pos.x = node.child("position").attribute("x").as_int();
	last_saved_pos.y = node.child("position").attribute("y").as_int();
	//saved_map = node.child("position").attribute("map").as_int();
	player_pos.x = last_saved_pos.x;
	player_pos.y = last_saved_pos.y;

	return true;
}

bool Player::Save(pugi::xml_node& node) const
{
	
	pugi::xml_node position = node.append_child("position");
	position.append_attribute("x").set_value(player_pos.x);
	position.append_attribute("y").set_value(player_pos.y);
	//position.append_attribute("map").set_value(App->scene->curr_map);
	return true;
}


void Player::Draw()
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

	case ATTACK:
		current_animation = &attack;
		break;

	default:
		current_animation = &idle;

	}

	//SDL_Rect r = current_animation->GetCurrentFrame();
	if (entity_x_dir == LEFT && on_liana==false) {
		App->render->Blit(texture, (int)player_pos.x + App->render->camera.x - collider->rect.w, (int)player_pos.y, &(current_animation->GetCurrentFrame()), NULL, NULL, SDL_FLIP_HORIZONTAL, 0,0);
	}
	else {
		App->render->Blit(texture, player_pos.x, player_pos.y, &(current_animation->GetCurrentFrame()));
	}
}

void Player::OnCollision(Collider* c1, Collider* c2) {




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
			if (c1->rect.y + c1->rect.h + (int)speed.y * dt_current + 1 > c2->rect.y && on_ground == false && c1->rect.y < c2->rect.y && (down_right_gid == 48 || down_right_gid == 63 || down_right_gid == 62 || down_right_gid == 198 || down_right_gid == 213 || down_right_gid == 212) && (down_left_gid == 48 || down_left_gid == 63 || down_left_gid == 62 || down_left_gid == 198 || down_left_gid == 213 || down_left_gid == 212)) {

				acceleration.y = 0.0f;
				speed.y = 0.0f;
				on_ground = true;
			}

			if (c1->rect.y + (int)speed.y * dt_current - 1 <= c2->rect.y + c2->rect.h && on_ground == false && c1->rect.y + c1->rect.h > c2->rect.y + c2->rect.h && (up_right_gid == 48 || up_right_gid == 63 || up_right_gid == 62 || up_right_gid == 198 || up_right_gid == 213 || up_right_gid == 212) && (up_left_gid == 48 || up_left_gid == 63 || up_left_gid == 62 || up_left_gid == 198 || up_left_gid == 213 || up_left_gid == 212)) {
				if (speed.y < 0.0f) {
					speed.y = -speed.y;
				}
				acceleration.y = gravity;
				player_pos.y = c2->rect.y + c2->rect.h + 1;

			}
			else
			{

				if (c1->rect.x + (int)speed.x * dt_current - 1 < c2->rect.x + c2->rect.w && (c1->rect.y > c2->rect.y || c1->rect.y > c2->rect.y - c1->rect.h * 8 / 10) && entity_x_dir == LEFT && c1->rect.x > c2->rect.x) {

					acceleration.x = 0.0f;
					speed.x = 0.0f;
					player_pos.x++;

				}
				else if (c1->rect.x + c1->rect.w + (int)speed.x * dt_current + 1 > c2->rect.x && (c1->rect.y > c2->rect.y || c1->rect.y > c2->rect.y - c1->rect.h * 8 / 10) && entity_x_dir == RIGHT && abs(c1->rect.x) < abs(c2->rect.x)) { //Remember to take this magic numbers off

					acceleration.x = 0.0f;
					speed.x = 0.0f;
					player_pos.x--;

				}
			}

			if (c2->type == COLLIDER_LEDGE && c1->rect.x + (int)speed.x * dt_current + 1 >= c2->rect.x + c2->rect.w && entity_x_dir == RIGHT && (down_right_gid == 62 || down_right_gid== 212|| down_right_gid == 0)) {
				on_ground = false;
			}
			else	if (c2->type == COLLIDER_LEDGE && c1->rect.x + c1->rect.w + (int)speed.x * dt_current - 1 <= c2->rect.x && entity_x_dir == LEFT && (down_left_gid == 62 || down_right_gid == 212 || down_left_gid == 0)) {
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

		if (c2->type == COLLIDER_DEAD && player_died == false && god_mode == false) {
			player_died = true;
			App->audio->PlayFx(lose_fx);
			speed = { 0.0f,0.0f };
			acceleration = { 0.0f,0.0f };

			player_pos.x = App->map->spawn_pos.x;
			player_pos.y = App->map->spawn_pos.y;
		}
	}

}


void Player::LoadVariablesXML(const pugi::xml_node& player_node) {

	pugi::xml_node variables = player_node.child("variables");
	
	limit_map = variables.child("limit_map").attribute("value").as_int();
	gravity = variables.child("gravity").attribute("value").as_float();
	acceleration_x = variables.child("acceleration_x").attribute("value").as_float();
	liana_speed = variables.child("liana_speed").attribute("value").as_float();
	max_acc_x = variables.child("max_acc_x").attribute("value").as_float();
	max_speed_x = variables.child("max_speed_x").attribute("value").as_float();
	jump_force = variables.child("jump_force").attribute("value").as_float();
	jump_force_liana = variables.child("jump_force_liana").attribute("value").as_float();
	collider_offset.x = variables.child("collider_offset").attribute("x").as_int();
	collider_offset.y = variables.child("collider_offset").attribute("y").as_int();
	collider_dimensions.x = variables.child("collider_dimensions").attribute("x").as_int();
	collider_dimensions.y = variables.child("collider_dimensions").attribute("y").as_int();
	spritesheet = variables.child("spritesheet").attribute("location").as_string();
	jump_fx_folder = variables.child("jump_fx_folder").attribute("location").as_string();
	lose_fx_folder = variables.child("lose_fx_folder").attribute("location").as_string();
	attack_time = variables.child("attack_time").attribute("value").as_int();

	/*jump_fx = App->audio->LoadFx(variables.child("jump_fx_folder").attribute("location").as_string());
	lose_fx = App->audio->LoadFx(variables.child("lose_fx_folder").attribute("location").as_string());
	texture = App->tex->Load(variables.child("spritesheet").attribute("location").as_string());*/
}

//void Player::SetAnimations(pugi::xml_node& config, Animation& animation)
//{
//	SDL_Rect coord;
//	for (; config; config = config.next_sibling("animation"))
//	{
//		coord.x = config.attribute("x").as_uint();
//		coord.y = config.attribute("y").as_uint();
//		coord.w = config.attribute("w").as_uint();
//		coord.h = config.attribute("h").as_uint();
//		animation.PushBack(coord);
//	}
//}

void Player::AnimationsApplyDt() {

	if (anim_speed_flag == false) {
		 idle_anim_speed = idle.speed;
		 run_anim_speed = run.speed;
		 jump_anim_speed = jump.speed;
		 climbing_anim_speed = climbing.speed;
		 //climbing_idle_anim_speed = climbing_idle.speed;
		 attack_anim_speed = attack.speed;

		 anim_speed_flag = true;
	}
	else
	{
		idle.speed = idle_anim_speed * dt_current;
		run.speed = run_anim_speed * dt_current;
		jump.speed = jump_anim_speed * dt_current;
		climbing.speed = climbing_anim_speed * dt_current;
		//climbing_idle.speed = climbing_idle_anim_speed * dt_current;
		attack.speed = attack_anim_speed * dt_current;
	}
	LOG("dt = %f", dt_current);
}