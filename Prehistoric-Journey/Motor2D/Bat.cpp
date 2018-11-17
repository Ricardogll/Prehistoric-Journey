#include "Bat.h"
#include "j1App.h"
#include "j1Collision.h"
#include "j1PathFinding.h"
#include "j1Entities.h"
#include "p2Log.h"
#include "p2DynArray.h"
#include "Player.h"
#include "j1Map.h"

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
	}
	state = IDLE;
	entity_x_dir = RIGHT;
	last_pos = position;

	
}

Bat::Bat(int x, int y, EntityTypes type) :Entity(x, y, type) { //DO CONSTRUCTOR THAT DOESNT NEED XML_NODE AND CALL IT IN ENTITIES WHEN CREATING NEW BAT

	/*pugi::xml_node node_entity = config.child("bat");

	if (node_entity != NULL) {
		LoadVariablesXML(node_entity);

		pugi::xml_node animations = node_entity.child("animations");
		

		SetAnimations(animations.child("fly").child("animation"), run);
		run.speed = animations.child("fly").attribute("speed").as_float();
		run.loop = animations.child("fly").attribute("loop").as_bool();

		SetAnimations(animations.child("death").child("animation"), death);
		death.speed = animations.child("death").attribute("speed").as_float();
		death.loop = animations.child("death").attribute("loop").as_bool();

		texture = App->tex->Load(spritesheet.GetString());
	}
	state = IDLE;
	entity_x_dir = RIGHT;
	last_pos = position;

	collider = App->collision->AddCollider({ (int)position.x + collider_offset.x, (int)position.y + collider_offset.x, collider_dimensions.x, collider_dimensions.y }, COLLIDER_ENEMY, (j1Module*)App->entities);
*/



}

Bat::~Bat() {}

void Bat::OnCollision(Collider* c1, Collider* c2) {}
void Bat::Update(float dt) {

	dt_current = dt;
	AnimationsApplyDt();
	
	fPoint player_pos = App->entities->GetPlayer()->position;
	float dist = position.DistanceNoSqrt(player_pos);

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

	last_pos = position;
	position.x += speed.x * dt_current;
	position.y += speed.y * dt_current;	

	collider->SetPos(position.x + collider_offset.x, position.y + collider_offset.y);
}

void Bat::Draw() {

	switch (state)
	{
	case RUN:
		current_animation = &run;
		break;

	case ATTACK:		
		break;

	default:
		current_animation = &run;
	}

	current_animation = &run;

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
		

		anim_speed_flag = true;
	}
	else
	{
		idle.speed = idle_anim_speed * dt_current;
		run.speed = run_anim_speed * dt_current;
		
	}
	

}

