#include "MiniTrex.h"
#include "j1App.h"
#include "j1Collision.h"
#include "j1PathFinding.h"
#include "j1Entities.h"
#include "p2Log.h"
#include "j1Map.h"
#include "j1Audio.h"
#include "Player.h"

MiniTrex::MiniTrex(int x, int y, pugi::xml_node& config, EntityTypes type) :Entity(x, y, type) {
	pugi::xml_node node_entity = config.child("mini-tyranosaur");

	if (node_entity != NULL) {
		LoadVariablesXML(node_entity);

		pugi::xml_node animations = node_entity.child("animations");
		/*SetAnimations(animations.child("idle").child("animation"), idle);
		idle.speed = animations.child("idle").attribute("speed").as_float();
		idle.loop = animations.child("idle").attribute("loop").as_bool();*/

		SetAnimations(animations.child("idle").child("animation"), idle);
		idle.speed = animations.child("idle").attribute("speed").as_float();
		idle.loop = animations.child("idle").attribute("loop").as_bool();

		SetAnimations(animations.child("walk").child("animation"), run);
		run.speed = animations.child("walk").attribute("speed").as_float();
		run.loop = animations.child("walk").attribute("loop").as_bool();

		SetAnimations(animations.child("death").child("animation"), death);
		death.speed = animations.child("death").attribute("speed").as_float();
		death.loop = animations.child("death").attribute("loop").as_bool();

		texture = App->tex->Load(spritesheet.GetString());
		idle_sound = App->audio->LoadFx(idle_sound_folder.GetString());
	}
	state = IDLE;
	entity_x_dir = RIGHT;

	collider = App->collision->AddCollider({ (int)position.x + collider_offset.x, (int)position.y + collider_offset.y, collider_dimensions.x, collider_dimensions.y }, COLLIDER_ENEMY, (j1Module*)App->entities);
	//soundtimer.Start();
}
MiniTrex::~MiniTrex() {}

void MiniTrex::OnCollision(Collider* c1, Collider* c2) {}


void MiniTrex::Update(float dt) {
	dt_current = dt;
	AnimationsApplyDt();

	fPoint player_pos = App->entities->GetPlayer()->position;
	float dist = position.DistanceNoSqrt(player_pos);

	if (soundtimer.Read() > 4000 && InsideCamera(position) == true) {
		App->audio->PlayFx(idle_sound, 1);
		soundtimer.Start();
	}
	LOG("TIMER %u", soundtimer.Read());
	if (position.DistanceNoSqrt(player_pos) < 90000 && position.DistanceNoSqrt(player_pos) > -90000) { // put this in xml as pathfinding_radius or something
		//make timer so it happens once every 0.5sec or so

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
			}
		}
	}
	else {
		speed = { 0.0f,0.0f };
	}

	speed.y = 0.0f;
	last_pos = position;
	position.x += speed.x * dt_current;
	position.y += speed.y * dt_current;

	collider->SetPos(position.x + collider_offset.x, position.y + collider_offset.y);
}
void MiniTrex::Draw() {
	switch (state)
	{
	case IDLE:
		current_animation = &idle;
		break;

	case RUN:
		current_animation = &run;
		break;

	default:
		current_animation = &idle;

	}

	current_animation = &idle;

	if (entity_x_dir == LEFT) {
		App->render->Blit(texture, (int)position.x + App->render->camera.x - collider->rect.w, (int)position.y, &(current_animation->GetCurrentFrame()), NULL, NULL, SDL_FLIP_HORIZONTAL, 0, 0);
	}
	else {
		App->render->Blit(texture, position.x, position.y, &(current_animation->GetCurrentFrame()));
	}
}


bool MiniTrex::Load(pugi::xml_node&) { return true; }
bool MiniTrex::Save(pugi::xml_node&) const { return true; }
void MiniTrex::AnimationsApplyDt() {

	if (anim_speed_flag == false) {
		idle_anim_speed = idle.speed;
		run_anim_speed = run.speed;


		anim_speed_flag = true;
	}
	else
	{
		idle.speed = idle_anim_speed * dt_current;
		run.speed = run_anim_speed * dt_current;

	}
}