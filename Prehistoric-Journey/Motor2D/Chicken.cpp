#include "Chicken.h"
#include "j1App.h"
#include "j1Collision.h"
#include "j1Entities.h"
#include "p2Log.h"
//#include "p2DynArray.h"
//#include "Player.h"
//#include "j1Map.h"
#include "j1Audio.h"
#include "Brofiler/Brofiler.h"
//#include <time.h>
#include "j1Scene.h"

Chicken::Chicken(int x, int y, pugi::xml_node& config, EntityTypes type) :Entity(x, y, type)
{
	pugi::xml_node node_entity = config.child("chicken");

	if (node_entity != NULL) {
		LoadVariablesXML(node_entity);

		pugi::xml_node animations = node_entity.child("animations");

		SetAnimations(animations.child("idle").child("animation"), idle);
		idle.speed = animations.child("idle").attribute("speed").as_float();
		idle.loop = animations.child("idle").attribute("loop").as_bool();

		texture = App->tex->Load(spritesheet.GetString());
		collider = App->collision->AddCollider({ (int)position.x + collider_offset.x, (int)position.y + collider_offset.y, collider_dimensions.x, collider_dimensions.y }, COLLIDER_COLLECTABLE, (j1Module*)App->entities);
	}
	state = IDLE;
}

Chicken::~Chicken()
{
}

void Chicken::Update(float dt)
{
	dt_current = dt;
	AnimationsApplyDt();
}

void Chicken::Draw()
{
	current_animation = &idle;

	App->render->Blit(texture, position.x, position.y, &(current_animation->GetCurrentFrame()));
}

void Chicken::AnimationsApplyDt()
{
	if (anim_speed_flag == false) {
		idle_anim_speed = idle.speed;

		anim_speed_flag = true;
	}
	else
	{
		idle.speed = idle_anim_speed * dt_current;
	}
}

void Chicken::OnCollision(Collider * c1, Collider * c2)
{
	if (c2->type == COLLIDER_PLAYER) {
		/*App->collision->EraseCollider(collider);
		App->tex->UnLoad(texture);*/
		App->scene->c_score++;
		if (App->scene->c_score >= 5 && App->scene->c_score < 10)
			App->scene->multiplier = 2;
		else if (App->scene->c_score >= 10)
			App->scene->multiplier = 3;

		if (App->scene->multiplier == 1)
			App->scene->score += 50;
		else if (App->scene->multiplier == 2)
			App->scene->score += 100;
		else
			App->scene->score += 150;
		this->to_destroy = true;
	}
}