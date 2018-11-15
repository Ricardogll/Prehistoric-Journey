#include "Bat.h"
#include "j1App.h"
#include "j1Collision.h"
#include "j1PathFinding.h"
#include "j1Entities.h"
#include "p2Log.h"


Bat::Bat(int x, int y, pugi::xml_node& config, EntityTypes type) :Entity(x, y, type) {
}
Bat::~Bat() {}

void Bat::OnCollision(Collider* c1, Collider* c2) {}
void Bat::Update(float dt) {
	LOG("AAA");
}
void Bat::Draw() {}
bool Bat::Load(pugi::xml_node&) { return true; }
bool Bat::Save(pugi::xml_node&) const { return true; }
void Bat::AnimationsApplyDt() {}