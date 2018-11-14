#include "j1App.h"
#include "j1Entities.h"
#include "Entity.h"
#include "p2Log.h"
#include "Player.h"
//#include


j1Entities::j1Entities() {
	name = "entities";
}


j1Entities::~j1Entities() {

}

bool j1Entities::Awake(pugi::xml_node& config) {
	LOG("Loading entities");

	return true;
}

bool j1Entities::Start() {

	bool ret = true;

	SpawnEntity(100, 100, EntityTypes::PLAYER);

	return ret;
}

bool j1Entities::PreUpdate() {
	//do deletes if to_destroy

	return true;
}

bool j1Entities::Update(float dt) {

	for (int i = 0; i < entities.Count(); i++) {
		if (entities[i] != nullptr) {
			entities[i]->Update(dt);
		}
	}
	for (int i = 0; i < entities.Count(); i++) {
		if (entities[i] != nullptr) {
			entities[i]->Draw();//put each entity sprite here?
		}
	}

	return true;
}

bool j1Entities::CleanUp() {
	LOG("CleanUp entities");

	//do deletes

	return true;
}


bool j1Entities::SpawnEntity(int x, int y, EntityTypes type) {

	bool ret = false;
	
	switch (type) {

	case EntityTypes::PLAYER: {
		Player* player = new Player(x, y, EntityTypes::PLAYER);
		entities.PushBack(player);
		pugi::xml_document config_doc;
		pugi::xml_node* config = &App->LoadConfig(config_doc);	//IMPROVE THIS!! 
		player->Awake(config->child("player"));
		ret = true;
		break;
	}
	case EntityTypes::TREX: {


		ret = true;
		break;
	}
	case EntityTypes::MINI_TREX: {


		ret = true;
		break;
	}
	case EntityTypes::PTERODACTYL: {


		ret = true;
		break;
	}
	case EntityTypes::BAT: {


		ret = true;
		break;
	}
	default:
		break;

	}

	return ret;

}


void j1Entities::OnCollision(Collider* c1, Collider* c2)
{
	/*for (uint i = 0; i < entities.Count(); ++i)
		if (entities[i] != nullptr && entities[i]->GetCollider() == c1)
			entities[i]->OnCollision(c2);*/
}


bool j1Entities::Load(pugi::xml_node&) { return true; }

bool j1Entities::Save(pugi::xml_node&) const { return true; }


