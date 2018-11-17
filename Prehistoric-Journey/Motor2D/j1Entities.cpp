#include "j1App.h"
#include "j1Entities.h"
#include "Entity.h"
#include "p2Log.h"
#include "Player.h"
#include "Bat.h"
#include "MiniTrex.h"
#include "Brofiler/Brofiler.h"




j1Entities::j1Entities() {
	name = "entities";
}


j1Entities::~j1Entities() {

}

bool j1Entities::Awake(pugi::xml_node& config) {
		
	/*pugi::xml_document	config_file;
	node = &App->LoadConfig(config_file);
	node = node->child("entities");*/
	

	return true;
}

bool j1Entities::Start() {

	bool ret = true;

	SpawnEntity(0, 0, EntityTypes::PLAYER);
	SpawnEntity(150, 200, EntityTypes::BAT);
	SpawnEntity(900, 300, EntityTypes::BAT);
	SpawnEntity(35*32, 12*32, EntityTypes::MINI_TREX);

	return ret;
}

bool j1Entities::PreUpdate() {
	//do deletes if to_destroy
	BROFILER_CATEGORY("PreUpdate Entities", Profiler::Color::DeepPink)
		
		for (uint i = 0u; i < entities.Count(); i++) {
			
			if (entities[i]->to_destroy) {
				delete(entities[i]);

				entities[i] = nullptr;

				if (!entities.Delete(i)) 
					return false;

			}
		}
	return true;
}

bool j1Entities::Update(float dt) {
	BROFILER_CATEGORY("Update Entities", Profiler::Color::MediumSpringGreen)

	for (uint i = 0u; i < entities.Count(); i++) {
		if (entities[i] != nullptr) {
			entities[i]->Update(dt);
		}
	}
	for (uint i = 0u; i < entities.Count(); i++) {
		if (entities[i] != nullptr) {
			entities[i]->Draw();
		}
	}

	return true;
}

bool j1Entities::CleanUp() {
	LOG("CleanUp entities");

	for (uint i = 0u; i < entities.Count(); i++) {

		if (entities[i] != nullptr) {
			delete(entities[i]);

			entities[i] = nullptr;

			if (!entities.Delete(i))
				return false;

		}
	}
	entities.Clear();

	return true;
}


bool j1Entities::SpawnEntity(int x, int y, EntityTypes type) {

	bool ret = false;
	
	pugi::xml_document config_doc;
	pugi::xml_node config = App->LoadConfig(config_doc);	


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

		MiniTrex* mini_trex = new MiniTrex(x, y, config.child("entities"), EntityTypes::MINI_TREX);
		entities.PushBack(mini_trex);

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

bool j1Entities::Load(pugi::xml_node& entity_node) {


	//for (uint i = 0; i < entities.Count(); i++) {
	//	if (entities[i] != nullptr) {
	//		entities[i]->Load(entity_node);
	//	}
	//}

	for (uint i = 0; i < entities.Count(); i++) {
		if (entities[i] != nullptr && entities[i]->type != EntityTypes::PLAYER) {
			entities[i]->to_destroy = true;
		}
	}
	if(GetPlayer())
		GetPlayer()->Load(entity_node);

	 
	for (pugi::xml_node aux = entity_node.child("mini-tyranosaur"); aux; aux = aux.next_sibling("mini-tyranosaur")) {
		SpawnEntity(aux.child("position").attribute("x").as_int(), aux.child("position").attribute("y").as_int(), EntityTypes::MINI_TREX);
	}


	for (pugi::xml_node aux = entity_node.child("bat"); aux; aux = aux.next_sibling("bat")) {
		SpawnEntity(aux.child("position").attribute("x").as_int(), aux.child("position").attribute("y").as_int(), EntityTypes::BAT);
	}

	return true; 
}

bool j1Entities::Save(pugi::xml_node& entity_node) const {
	/*if (GetPlayer() != nullptr)
		GetPlayer()->Save(entity_node);*/

	for (uint i = 0; i < entities.Count(); i++) {
		if (entities[i] != nullptr) {
			entities[i]->Save(entity_node);
		}
	}

	return true;
}
