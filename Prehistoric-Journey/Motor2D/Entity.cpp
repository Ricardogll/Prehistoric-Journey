#include"j1App.h"
#include"Entity.h"
#include"j1Collision.h"
#include"j1Render.h"
#include "j1Map.h"



Entity::Entity(int x, int y, EntityTypes type) :position(x, y), type(type) {}

Entity::~Entity() {
	
	
	if (collider != nullptr) {
		App->collision->EraseCollider(collider);
		collider = nullptr;
	}
	App->tex->UnLoad(texture);


}

void Entity::Draw() {

}

void Entity::OnCollision(Collider* c1, Collider* c2){

	
		
}

void Entity::SetAnimations(pugi::xml_node& config, Animation& animation)
{
	SDL_Rect coord;
	for (; config; config = config.next_sibling("animation"))
	{
		coord.x = config.attribute("x").as_uint();
		coord.y = config.attribute("y").as_uint();
		coord.w = config.attribute("w").as_uint();
		coord.h = config.attribute("h").as_uint();
		animation.PushBack(coord);
	}
}

void Entity::LoadVariablesXML(const pugi::xml_node& entity_node) {

	pugi::xml_node variables = entity_node.child("variables");

	
	
	collider_offset.x = variables.child("collider_offset").attribute("x").as_int();
	collider_offset.y = variables.child("collider_offset").attribute("y").as_int();
	collider_dimensions.x = variables.child("collider_dimensions").attribute("x").as_int();
	collider_dimensions.y = variables.child("collider_dimensions").attribute("y").as_int();
	spritesheet = variables.child("spritesheet").attribute("location").as_string();
	wait_pf = variables.child("wait_pf").attribute("value").as_float();
	gravity = variables.child("gravity").attribute("value").as_float();
	idle_sound_folder = variables.child("sounds").attribute("idle_location").as_string();
	death_sound_folder = variables.child("sounds").attribute("death_location").as_string();
	radar = variables.child("radar").attribute("value").as_int();
	sound_time = variables.child("sound_time").attribute("value").as_uint();
	hear_dist = variables.child("hear_dist").attribute("value").as_uint();

}

fPoint Entity::SpeedNeededFromTo(iPoint& from, iPoint& to) const
{

	fPoint distance;
	distance.x = to.x - from.x;
	distance.y = to.y - from.y;
	double mod = sqrt(distance.x*distance.x + distance.y*distance.y);
	if (mod != 0) {
		distance.x /= mod;
		distance.y /= mod;

		distance.x *= moving_speed;
		distance.y *= moving_speed;

		return distance;
	}
	return fPoint(0.0f, 0.0f);
}

 bool Entity::Save(pugi::xml_node& entity_node) const { 
	 
	 pugi::xml_node aux;

	 switch (type) {

	 case EntityTypes::BAT:
		  aux = entity_node.append_child("bat");
		 break;

	 case EntityTypes::MINI_TREX:
		  aux = entity_node.append_child("mini-tyranosaur");
		 break;

	 case EntityTypes::PLAYER:
	 default:
		 return true;
	 }

	 aux = aux.append_child("position");
	 aux.append_attribute("x").set_value(position.x);
	 aux.append_attribute("y").set_value(position.y);

	 bool dead_aux = true;
	 
	 if (state != DEATH)
		 dead_aux = false;
	 
	 aux.append_attribute("dead").set_value(dead_aux);
	 
	 return true; 

 }

 bool Entity::OnScreen(int x)  {

	 return (-App->render->camera.x <= x && -App->render->camera.x + App->render->camera.w >= x);
 }
