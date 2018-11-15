#include "MiniTrex.h"
#include "j1App.h"
#include "j1Collision.h"
#include "j1PathFinding.h"
#include "j1Entities.h"
#include "p2Log.h"


MiniTrex::MiniTrex(int x, int y, pugi::xml_node& config, EntityTypes type) :Entity(x, y, type) {
}
MiniTrex::~MiniTrex() {}

void MiniTrex::OnCollision(Collider* c1, Collider* c2) {}
void MiniTrex::Update(float dt) {
	LOG("AAA");
}
void MiniTrex::Draw() {}
bool MiniTrex::Load(pugi::xml_node&) { return true; }
bool MiniTrex::Save(pugi::xml_node&) const { return true; }
void MiniTrex::AnimationsApplyDt() {}