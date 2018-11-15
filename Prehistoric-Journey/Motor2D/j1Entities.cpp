#include "j1App.h"
#include "j1Entities.h"
#include "Entity.h"
#include "p2Log.h"
#include "Player.h"
#include "Bat.h"
//#include


j1Entities::j1Entities() {
	name = "entities";
}


j1Entities::~j1Entities() {

}

bool j1Entities::Awake(pugi::xml_node& config) {
	LOG("Loading entities");
	entities_node = *(&config);
	//entities_node = config;
	
	

	return true;
}

bool j1Entities::Start() {

	bool ret = true;

	SpawnEntity(100, 100, EntityTypes::PLAYER);
	SpawnEntity(50, 50, EntityTypes::BAT);

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
	
	pugi::xml_document config_doc;
	pugi::xml_node config = App->LoadConfig(config_doc);	//IMPROVE THIS!! 
	//player->Awake(config->child("player"));

	switch (type) {

	case EntityTypes::PLAYER: {
		

		Player* player = new Player(x, y, config.child("entities"), EntityTypes::PLAYER);
		entities.PushBack(player);
		
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

		Bat* bat = new Bat(x, y, config.child("entities"), EntityTypes::BAT);
		entities.PushBack(bat);

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
	for (uint i = 0; i < entities.Count(); ++i)
		if (entities[i] != nullptr && entities[i]->collider == c1)
			entities[i]->OnCollision(c1, c2);
}

Player* j1Entities::GetPlayer() const {

	for (uint i = 0; i < entities.Count(); ++i)
	{
		if (entities[i] != nullptr)
		{
			if (entities[i]->type == EntityTypes::PLAYER)
				return (Player*)entities[i];
		}
	}

	return nullptr;

}

bool j1Entities::Load(pugi::xml_node& player_node) {

	if (GetPlayer() != nullptr)
		GetPlayer()->Load(player_node);
	return true; 
}

bool j1Entities::Save(pugi::xml_node& player_node) const {
	if (GetPlayer() != nullptr)
		GetPlayer()->Save(player_node);
	return true;
}
