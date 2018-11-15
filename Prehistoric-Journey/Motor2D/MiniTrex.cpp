#include "MiniTrex.h"
#include "j1App.h"
#include "j1Collision.h"
#include "j1PathFinding.h"
#include "j1Entities.h"
#include "p2Log.h"


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
	}
	state = IDLE;
	entity_x_dir = RIGHT;
}
MiniTrex::~MiniTrex() {}

void MiniTrex::OnCollision(Collider* c1, Collider* c2) {}
void MiniTrex::Update(float dt) {
	dt_current = dt;
	AnimationsApplyDt();
}
void MiniTrex::Draw() {
	/*switch (state)
	{
	case IDLE:
		current_animation = &idle;
		break;

	case RUN:
		current_animation = &run;
		break;

	case JUMP:

		break;

	case LIANA:

		break;

	case LIANA_IDLE:

		break;

	case ATTACK:

		break;

	default:
		current_animation = &idle;

	}*/

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

void MiniTrex::LoadVariablesXML(const pugi::xml_node& entity_node) {

	pugi::xml_node variables = entity_node.child("variables");
	spritesheet = variables.child("spritesheet").attribute("location").as_string();

}