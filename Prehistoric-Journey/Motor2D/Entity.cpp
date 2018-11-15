#include"j1App.h"
#include"Entity.h"
#include"j1Collision.h"
#include"j1Entities.h"


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

