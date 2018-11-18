#include "Bat.h"
#include "j1App.h"
#include "j1Collision.h"
#include "j1PathFinding.h"
#include "j1Entities.h"
#include "p2Log.h"
#include "p2DynArray.h"
#include "Player.h"
#include "j1Map.h"
#include "j1Audio.h"
#include <time.h>

Bat::Bat(int x, int y, pugi::xml_node& config, EntityTypes type) :Entity(x, y, type) {

	pugi::xml_node node_entity = config.child("bat");

	if (node_entity != NULL) {
		LoadVariablesXML(node_entity);

		pugi::xml_node animations = node_entity.child("animations");
		/*SetAnimations(animations.child("idle").child("animation"), idle);
		idle.speed = animations.child("idle").attribute("speed").as_float();
		idle.loop = animations.child("idle").attribute("loop").as_bool();*/

		SetAnimations(animations.child("fly").child("animation"), run);
		run.speed = animations.child("fly").attribute("speed").as_float();
		run.loop = animations.child("fly").attribute("loop").as_bool();

		SetAnimations(animations.child("death").child("animation"), death);
		death.speed = animations.child("death").attribute("speed").as_float();
		death.loop = animations.child("death").attribute("loop").as_bool();

		texture = App->tex->Load(spritesheet.GetString());
		collider = App->collision->AddCollider({ (int)position.x + collider_offset.x, (int)position.y + collider_offset.y, collider_dimensions.x, collider_dimensions.y }, COLLIDER_ENEMY, (j1Module*)App->entities);
		idle_sound = App->audio->LoadFx(idle_sound_folder.GetString());
		death_sound = App->audio->LoadFx(death_sound_folder.GetString());
	}
	state = RUN;
	entity_x_dir = RIGHT;
	last_pos = position;
	srand(time(NULL));
}

Bat::~Bat() {}

void Bat::Update(float dt) {
	
	prev_pos = position;

	dt_current = dt;
	AnimationsApplyDt();
	
	fPoint player_pos = App->entities->GetPlayer()->position;
	float dist = position.DistanceNoSqrt(player_pos);

	if (soundtimer.Read() > 4000 && dist < 290000) {
		App->audio->PlayFx(idle_sound, 0);
		soundtimer.Start();
	}

	if (position.DistanceNoSqrt(player_pos) < 90000 && position.DistanceNoSqrt(player_pos) > -90000 && state != DEATH ) { // put this in xml as pathfinding_radius or something
		
		
		if (timer_pathfinding + wait_pf < SDL_GetTicks()) {
			if (App->pathfinding->CreatePath(App->map->WorldToMap(position.x, position.y), App->map->WorldToMap(player_pos.x, player_pos.y)) != -1) {
				path = App->pathfinding->GetLastPath();
				timer_pathfinding = SDL_GetTicks();

				if (iPoint(path->At(0)->x, path->At(0)->y) != App->map->WorldToMap(position.x, position.y))
					speed = SpeedNeededFromTo(App->map->WorldToMap(position.x, position.y), iPoint(path->At(0)->x, path->At(0)->y));
				else {
					speed = SpeedNeededFromTo(App->map->WorldToMap(position.x, position.y), iPoint(path->At(1)->x, path->At(1)->y));
					if (last_pos.x < position.x)
						entity_x_dir = RIGHT;
					else if (last_pos.x > position.x)
						entity_x_dir = LEFT;
				}
			}
			else {
				path = nullptr;
				LOG("PATH NOT FOUND BAT");
				speed = { 0.0f,0.0f };
			}
		}
	}
	else if (state == DEATH){
		speed = { 0.0f, gravity };
	}
	else {
		

		new_pos.x = rand() % 10;
		new_pos.y = rand() % 10;
		
		new_pos.x = (new_pos.x < 5) ? 1 : -1;
		new_pos.y = (new_pos.y < 5) ? 1 : -1;

		next_pos = App->map->WorldToMap(position.x, position.y);

		next_pos = { next_pos.x + new_pos.x, next_pos.y + new_pos.y};

		if (App->pathfinding->IsWalkable(next_pos)) {
			speed = SpeedNeededFromTo(App->map->WorldToMap(position.x, position.y), next_pos);
			if (last_pos.x < position.x)
				entity_x_dir = RIGHT;
			else if (last_pos.x > position.x)
				entity_x_dir = LEFT;
		}
		else
			speed = { 0.0f,0.0f };
	}

	last_pos = position;
	position.x += speed.x * dt_current;
	position.y += speed.y * dt_current;	

	collider->SetPos(position.x + collider_offset.x, position.y + collider_offset.y);

	
	on_ground = false;
}

void Bat::Draw() {

	switch (state)
	{
	case RUN:
		current_animation = &run;
		break;

	case DEATH:
		current_animation = &death;
		break;

	default:
		current_animation = &run;
	}

	if (entity_x_dir == LEFT ) {
		App->render->Blit(texture, (int)position.x + App->render->camera.x, (int)position.y, &(current_animation->GetCurrentFrame()), NULL, NULL, SDL_FLIP_HORIZONTAL, 0, 0);
	}
	else {
		App->render->Blit(texture, position.x, position.y, &(current_animation->GetCurrentFrame()));
	}

	
}




void Bat::AnimationsApplyDt() {

	if (anim_speed_flag == false) {
		idle_anim_speed = idle.speed;
		run_anim_speed = run.speed;
		death_anim_speed = death.speed;

		anim_speed_flag = true;
	}
	else
	{
		idle.speed = idle_anim_speed * dt_current;
		run.speed = run_anim_speed * dt_current;
		death.speed = death_anim_speed * dt_current;
	}
	

}


void Bat::OnCollision(Collider* c1, Collider* c2) {

	

	if (App->map->data.layers.end != nullptr) {


		MapLayer* layer_coll = App->map->data.layers.end->data;


		iPoint down_right = App->map->WorldToMap(position.x + collider->rect.w - collider_offset.x, position.y + collider->rect.h);
		iPoint down_left = App->map->WorldToMap(position.x + collider_offset.x, position.y + collider->rect.h);

		int down_right_gid=0;
		if (down_right.x >= 0 && down_right.x <= App->map->data.width && down_right.y >= 0 && down_right.y <= App->map->data.height)//If tile is outside the map dont ->Get(...) or error
			down_right_gid = layer_coll->Get(down_right.x, down_right.y);
		
		int down_left_gid = 0;
		if (down_left.x >= 0 && down_left.x <= App->map->data.width && down_left.y >= 0 && down_left.y <= App->map->data.height)
			down_left_gid = layer_coll->Get(down_left.x, down_left.y);


		iPoint up_right = App->map->WorldToMap(position.x + collider->rect.w - collider_offset.x, position.y);
		iPoint up_left = App->map->WorldToMap(position.x + collider_offset.x, position.y);

		int up_right_gid = 0;
		if (up_right.x >= 0 && up_right.x <= App->map->data.width && up_right.y >= 0 && up_right.y <= App->map->data.height)
			up_right_gid = layer_coll->Get(up_right.x, up_right.y);

		int up_left_gid = 0;
		if (up_left.x >= 0 && up_left.x <= App->map->data.width && up_left.y >= 0 && up_left.y <= App->map->data.height)
			up_left_gid = layer_coll->Get(up_left.x, up_left.y);

		if (c2->type == COLLIDER_WALL || c2->type == COLLIDER_LEDGE)
		{ //Using "(int)speed" to see if in the next update player will be inside the wall. Using +1 in case the float is shortened and we end up going inside the wall.
			//Touching floor
			if (c1->rect.y + c1->rect.h + (int)speed.y * dt_current + 1 > c2->rect.y && on_ground == false && c1->rect.y < c2->rect.y && (down_right_gid == 48 || down_right_gid == 63 || down_right_gid == 62 || down_right_gid == 198 || down_right_gid == 213 || down_right_gid == 212) && (down_left_gid == 48 || down_left_gid == 63 || down_left_gid == 62 || down_left_gid == 198 || down_left_gid == 213 || down_left_gid == 212)) {

				if (speed.y > 0.0f) 
				speed.y = -speed.y;

				position.y = prev_pos.y - 1;

				on_ground = true;
			}

			//Touching ceiling
			if (c1->rect.y + (int)speed.y * dt_current - 1 <= c2->rect.y + c2->rect.h && on_ground == false && c1->rect.y + c1->rect.h > c2->rect.y + c2->rect.h && (up_right_gid == 48 || up_right_gid == 63 || up_right_gid == 62 || up_right_gid == 198 || up_right_gid == 213 || up_right_gid == 212) && (up_left_gid == 48 || up_left_gid == 63 || up_left_gid == 62 || up_left_gid == 198 || up_left_gid == 213 || up_left_gid == 212)) {
				if (speed.y < 0.0f) {
					speed.y = -speed.y;
				}

				position.y = prev_pos.y + 1;

			}
			else
			{
				//Touching left
				if (c1->rect.x + (int)speed.x * dt_current - 1 < c2->rect.x + c2->rect.w && (c1->rect.y > c2->rect.y || c1->rect.y > c2->rect.y - c1->rect.h * 8 / 10) && entity_x_dir == LEFT && c1->rect.x > c2->rect.x) {

					if(speed.x < 0.0f)
						speed.x = -speed.x;
					position.x = prev_pos.x + 1;

				}//Touching right
				else if (c1->rect.x + c1->rect.w + (int)speed.x * dt_current + 1 > c2->rect.x && (c1->rect.y > c2->rect.y || c1->rect.y > c2->rect.y - c1->rect.h * 8 / 10) && entity_x_dir == RIGHT && abs(c1->rect.x) < abs(c2->rect.x)) { //Remember to take this magic numbers off

					if (speed.x > 0.0f)
						speed.x = -speed.x;
					position.x = prev_pos.x - 1;

				}
			}

			
		}
		
	}
	if (c2->type == COLLIDER_PLAYER_ATTACK) {
		App->audio->PlayFx(death_sound, 0);
		state = DEATH;
		App->collision->EraseCollider(collider);
	}
}