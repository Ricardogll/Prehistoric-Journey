#include "j1App.h"
#include "j1Entities.h"
#include "Entity.h"
#include "p2Log.h"
#include "Player.h"
#include "Bat.h"
#include "j1Scene.h"
#include "MiniTrex.h"
#include "Chicken.h"
#include "Brofiler/Brofiler.h"




j1Entities::j1Entities() {
	name = "entities";
}


j1Entities::~j1Entities() {
	to_create1.clear();
	to_create2.clear();
}

bool j1Entities::Awake(pugi::xml_node& config) {
		
	pugi::xml_node aux_node;
	
	//Saving entities positions for map 1
	for (aux_node = config.child("spawns1").child("bat"); aux_node; aux_node = aux_node.next_sibling("bat")) {
		
		to_create1.add(ToCreate(aux_node.attribute("x").as_int(), aux_node.attribute("y").as_int(), EntityTypes::BAT));
	}

	for (aux_node = config.child("spawns1").child("mini_trex"); aux_node; aux_node = aux_node.next_sibling("mini_trex")) {
	
		to_create1.add(ToCreate(aux_node.attribute("x").as_int(), aux_node.attribute("y").as_int(), EntityTypes::MINI_TREX));
	}

	for (aux_node = config.child("spawns1").child("chicken"); aux_node; aux_node = aux_node.next_sibling("chicken")) {

		to_create1.add(ToCreate(aux_node.attribute("x").as_int(), aux_node.attribute("y").as_int(), EntityTypes::CHICKEN));
	}

	//Saving entities positions for map 2
	for (aux_node = config.child("spawns2").child("bat"); aux_node; aux_node = aux_node.next_sibling("bat")) {

		to_create2.add(ToCreate(aux_node.attribute("x").as_int(), aux_node.attribute("y").as_int(), EntityTypes::BAT));
	}

	for (aux_node = config.child("spawns2").child("mini_trex"); aux_node; aux_node = aux_node.next_sibling("mini_trex")) {

		to_create2.add(ToCreate(aux_node.attribute("x").as_int(), aux_node.attribute("y").as_int(), EntityTypes::MINI_TREX));
	}

	for (aux_node = config.child("spawns2").child("chicken"); aux_node; aux_node = aux_node.next_sibling("chicken")) {

		to_create1.add(ToCreate(aux_node.attribute("x").as_int(), aux_node.attribute("y").as_int(), EntityTypes::CHICKEN));
	}	

	return true;
}

void j1Entities::SetEnemies(uint map) {

	p2List_item<ToCreate>* item;

	switch (map) {

	case 1: {

		item = to_create1.start;

		for (item; item != nullptr; item = item->next) {

			switch (item->data.type) {
			case EntityTypes::BAT:
				SpawnEntity(item->data.x, item->data.y, EntityTypes::BAT);

				break;
			case EntityTypes::MINI_TREX:
				SpawnEntity(item->data.x, item->data.y, EntityTypes::MINI_TREX);

				break;
			case EntityTypes::CHICKEN:
				SpawnEntity(item->data.x, item->data.y, EntityTypes::CHICKEN);

				break;
			default:
				break;
			}
		}
		
		break;
	}
	case 2: {

		item = to_create2.start;

		for (item; item != nullptr; item = item->next) {

			switch (item->data.type) {
			case EntityTypes::BAT:
				SpawnEntity(item->data.x, item->data.y, EntityTypes::BAT);

				break;
			case EntityTypes::MINI_TREX:
				SpawnEntity(item->data.x, item->data.y, EntityTypes::MINI_TREX);

				break;
			case EntityTypes::CHICKEN:
				SpawnEntity(item->data.x, item->data.y, EntityTypes::CHICKEN);

				break;
			default:
				break;
			}
		}
		break;
	}
	default:
		break;

	}
}


bool j1Entities::Start() {

	bool ret = true;

	SpawnEntity(0, 0, EntityTypes::PLAYER);
	
	p2List_item<ToCreate>* item = to_create1.start;

	for (item; item != nullptr; item = item->next) {

		switch (item->data.type) {
		case EntityTypes::BAT:
			SpawnEntity(item->data.x, item->data.y, EntityTypes::BAT);

			break;
		case EntityTypes::MINI_TREX:
			SpawnEntity(item->data.x, item->data.y, EntityTypes::MINI_TREX);

			break;
		case EntityTypes::CHICKEN:
			SpawnEntity(item->data.x, item->data.y, EntityTypes::CHICKEN);

			break;
		default:
			break;
		}
	}

	return ret;
}

bool j1Entities::PreUpdate() {
	//do deletes if to_destroy
	BROFILER_CATEGORY("PreUpdate Entities", Profiler::Color::DeepPink)
		
	//https://stackoverflow.com/questions/25569985/sdl-invalid-texture-error-on-sdl-destroytexture TODO: Check if this is happening
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
	
	if (App->scene->pause)
		dt = 0;

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

bool j1Entities::DeleteEnemies() {

	for (uint i = 0u; i < entities.Count(); i++) {

		if (entities[i] != nullptr) {
			if (entities[i]->type != EntityTypes::PLAYER) {
				
				entities[i]->to_destroy = true;

			}
		}
	}
	
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
	case EntityTypes::MINI_TREX: {

		MiniTrex* mini_trex = new MiniTrex(x, y, config.child("entities"), EntityTypes::MINI_TREX);
		entities.PushBack(mini_trex);

		ret = true;
		break;
	}
	case EntityTypes::BAT: {

		Bat* bat = new Bat(x, y, config.child("entities"), EntityTypes::BAT);
		entities.PushBack(bat);

		ret = true;
		break;
	}
	case EntityTypes::CHICKEN: {

		Chicken* chicken = new Chicken(x, y, config.child("entities"), EntityTypes::CHICKEN);
		entities.PushBack(chicken);

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

	for (uint i = 0; i < entities.Count(); i++) {
		if (entities[i] != nullptr && entities[i]->type != EntityTypes::PLAYER) {
			entities[i]->to_destroy = true;
		}
	}
	if(GetPlayer())
		GetPlayer()->Load(entity_node);

	bool dead_aux = false;
	for (pugi::xml_node aux = entity_node.child("mini-tyranosaur"); aux; aux = aux.next_sibling("mini-tyranosaur")) {
		dead_aux = aux.child("position").attribute("dead").as_bool();
		if(!dead_aux)
			SpawnEntity(aux.child("position").attribute("x").as_int(), -15+aux.child("position").attribute("y").as_int(), EntityTypes::MINI_TREX);
	}


	for (pugi::xml_node aux = entity_node.child("bat"); aux; aux = aux.next_sibling("bat")) {
		dead_aux = aux.child("position").attribute("dead").as_bool();
		if (!dead_aux)
			SpawnEntity(aux.child("position").attribute("x").as_int(), aux.child("position").attribute("y").as_int(), EntityTypes::BAT);
	}

	for (pugi::xml_node aux = entity_node.child("chicken"); aux; aux = aux.next_sibling("chicken")) {
		dead_aux = aux.child("position").attribute("dead").as_bool();
		if (!dead_aux)
			SpawnEntity(aux.child("position").attribute("x").as_int(), aux.child("position").attribute("y").as_int(), EntityTypes::CHICKEN);
	}

	return true; 
}

bool j1Entities::Save(pugi::xml_node& entity_node) const {

	for (uint i = 0; i < entities.Count(); i++) {
		if (entities[i] != nullptr) {
			entities[i]->Save(entity_node);
		}
	}

	return true;
}
