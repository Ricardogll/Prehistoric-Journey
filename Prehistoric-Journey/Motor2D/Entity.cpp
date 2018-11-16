#include"j1App.h"
#include"Entity.h"
#include"j1Collision.h"



Entity::Entity(int x, int y, EntityTypes type) :position(x, y), type(type) {}

Entity::~Entity() {
	
	//delete coll
	if (collider != nullptr) {
		App->collision->EraseCollider(collider);
		collider = nullptr;
	}
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