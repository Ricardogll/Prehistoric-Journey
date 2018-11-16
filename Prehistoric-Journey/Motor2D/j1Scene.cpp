#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "Player.h"
#include "j1Collision.h"
#include "j1FadeToBlack.h"
#include "j1Entities.h"
#include "j1PathFinding.h"
#include "Brofiler/Brofiler.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	pugi::xml_node variables = config.child("music");

	music_map1 = (variables.child("music_map1").attribute("location").as_string());
	music_map2 = variables.child("music_map2").attribute("location").as_string();

	


	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	


	if (curr_map == NO_MAP)
	{
		App->map->Load("Jungle.tmx");
		curr_map = MAP_1;
		App->map->setColliders();

		int w, h;//change to differentiate between maps
		uchar* data = NULL;
		if (App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	if (App->entities->GetPlayer() == nullptr) {
		if (App->audio->active)
			App->audio->PlayMusic(music_map1.GetString(), 0.5f);			
	}
	else if (App->entities->GetPlayer()->player_died == false) {
		

		switch (curr_map) {
		case MAP_1:
			if (App->audio->active)
				App->audio->PlayMusic(music_map1.GetString(), 0.5f);
			break;
		case MAP_2:
			App->audio->PlayMusic(music_map2.GetString(), 0.5f);
			break;

		}

	}
	else if (App->entities->GetPlayer()->player_died)
		App->entities->GetPlayer()->player_died = false;


	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	BROFILER_CATEGORY("PreUpdate Scene", Profiler::Color::DeepPink)
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	BROFILER_CATEGORY("Update Scene", Profiler::Color::MediumSpringGreen)
	if (App->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN) {
		App->audio->RaiseVolumeMusic();
		App->audio->RaiseVolumeFx();
	}

	if (App->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN) {
		App->audio->DecreaseVolumeMusic();
		App->audio->DecreaseVolumeFx();
	}

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN && App->fade->IsFading()==false) {
		
		
			App->LoadGame();
			if (curr_map != App->entities->GetPlayer()->saved_map)
			{
			switch (App->entities->GetPlayer()->saved_map) {
			case 1:
				curr_map = MAP_1;
				App->fade->FadeToBlack(this, this, 2.0f);
				App->map->CleanUp();
				App->collision->CleanUpMap();
				App->map->Load("Jungle.tmx");
				App->map->setColliders();
				App->entities->GetPlayer()->player_pos.x = App->entities->GetPlayer()->last_saved_pos.x;
				App->entities->GetPlayer()->player_pos.y = App->entities->GetPlayer()->last_saved_pos.y;
				is_fade = true;
				break;
			case 2:
				curr_map = MAP_2;
				App->fade->FadeToBlack(this, this, 2.0f);
				App->map->CleanUp();
				App->collision->CleanUpMap();
				App->map->Load("Cave.tmx");
				App->map->setColliders();
				App->entities->GetPlayer()->player_pos.x = App->entities->GetPlayer()->last_saved_pos.x;
				App->entities->GetPlayer()->player_pos.y = App->entities->GetPlayer()->last_saved_pos.y;
				is_fade = true;
				break;
			default:
				break;
			}
		}

	}

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN && App->fade->IsFading() == false) {
		App->SaveGame();
		App->entities->GetPlayer()->saved_map = curr_map;
	}
	
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN && App->fade->IsFading() == false)
	{
		App->entities->GetPlayer()->change_map = false;

		switch (curr_map) {
		case MAP_1:

			curr_map = MAP_1;
			App->fade->FadeToBlack(this, this, 2.0f);
			App->entities->GetPlayer()->player_pos.x = App->map->spawn_pos.x;
			App->entities->GetPlayer()->player_pos.y = App->map->spawn_pos.y;
			is_fade = true;
			break;

		case MAP_2:
			
			curr_map = MAP_1;
			App->fade->FadeToBlack(this, this, 2.0f);
			App->map->CleanUp();
			App->collision->CleanUpMap();
			App->map->Load("Jungle.tmx");
			App->map->setColliders();
			App->entities->GetPlayer()->player_pos.x = App->map->spawn_pos.x;
			App->entities->GetPlayer()->player_pos.y = App->map->spawn_pos.y;
			is_fade = true;
			break;
		}

		
	}

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN && App->fade->IsFading() == false)
	{
		switch (curr_map) {
		case MAP_2:

			curr_map = MAP_2;
			App->fade->FadeToBlack(this, this, 2.0f);
			App->entities->GetPlayer()->player_pos.x = App->map->spawn_pos.x;
			App->entities->GetPlayer()->player_pos.y = App->map->spawn_pos.y;
			is_fade = true;
			break;

		case MAP_1:

			curr_map = MAP_2;
			App->fade->FadeToBlack(this, this, 2.0f);
			App->map->CleanUp();
			App->collision->CleanUpMap();
			App->map->Load("Cave.tmx");
			App->map->setColliders();
			App->entities->GetPlayer()->player_pos.x = App->map->spawn_pos.x;
			App->entities->GetPlayer()->player_pos.y = App->map->spawn_pos.y;
			is_fade = true;
			break;
		}
	}

	if (App->entities->GetPlayer()->change_map) {
		App->entities->GetPlayer()->change_map = false;
		switch (curr_map) {
		case MAP_2:

			curr_map = MAP_1;
			App->fade->FadeToBlack(this, this, 2.0f);
			App->map->CleanUp();
			App->collision->CleanUpMap();
			App->map->Load("Jungle.tmx");
			App->map->setColliders();
			App->entities->GetPlayer()->player_pos.x = App->map->spawn_pos.x;
			App->entities->GetPlayer()->player_pos.y = App->map->spawn_pos.y;
			is_fade = true;
			break;

		case MAP_1:

			curr_map = MAP_2;
			App->fade->FadeToBlack(this, this, 2.0f);
			App->map->CleanUp();
			App->collision->CleanUpMap();
			App->map->Load("Cave.tmx");
			App->map->setColliders();
			App->entities->GetPlayer()->player_pos.x = App->map->spawn_pos.x;
			App->entities->GetPlayer()->player_pos.y = App->map->spawn_pos.y;
			is_fade = true;
			break;
		}
	}

	

	if (App->entities->GetPlayer()->player_died) {
		
		App->fade->FadeToBlack(this, this, 2.0f);
	}


	App->map->Draw();
	
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	BROFILER_CATEGORY("PostUpdate Scene", Profiler::Color::RoyalBlue)
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
