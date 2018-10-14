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
#include "j1Player.h"
#include "j1Collision.h"
#include "j1FadeToBlack.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

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

		
	}

	if (App->player->player_died == false) {
		

		switch (curr_map) {
		case MAP_1:
			if (App->audio->active)
				App->audio->PlayMusic("audio/music/theme-1.ogg", 0.5f);
			break;
		case MAP_2:
			App->audio->PlayMusic("audio/music/theme-2.ogg", 0.5f);
			break;

		}
	}

	if(App->player->player_died)
		App->player->player_died = false;

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN) {
		App->audio->RaiseVolumeMusic();
		App->audio->RaiseVolumeFx();
	}

	if (App->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN) {
		App->audio->DecreaseVolumeMusic();
		App->audio->DecreaseVolumeFx();
	}

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN && App->fade->IsFading()==false) {
		
		if(curr_map == App->player->saved_map)
			App->LoadGame();
		else
		{
			switch (App->player->saved_map) {
			case 1:
				curr_map = MAP_1;
				App->fade->FadeToBlack(this, this, 2.0f);
				App->map->CleanUp();
				App->collision->CleanUpMap();
				App->map->Load("Jungle.tmx");
				App->map->setColliders();
				App->player->player_pos.x = App->player->last_saved_pos.x;
				App->player->player_pos.y = App->player->last_saved_pos.y;
				is_fade = true;
				break;
			case 2:
				curr_map = MAP_2;
				App->fade->FadeToBlack(this, this, 2.0f);
				App->map->CleanUp();
				App->collision->CleanUpMap();
				App->map->Load("Cave.tmx");
				App->map->setColliders();
				App->player->player_pos.x = App->player->last_saved_pos.x;
				App->player->player_pos.y = App->player->last_saved_pos.y;
				is_fade = true;
				break;
			default:
				break;
			}
		}

	}

	if(App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN && App->fade->IsFading() == false)
		App->SaveGame();

	/*if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y -= 10;

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y += 10;

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x -= 10;

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x += 10;*/

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN && App->fade->IsFading() == false)
	{
		App->player->change_map = false;

		switch (curr_map) {
		case MAP_1:

			curr_map = MAP_1;
			App->fade->FadeToBlack(this, this, 2.0f);
			App->player->player_pos.x = App->map->spawn_pos.x;
			App->player->player_pos.y = App->map->spawn_pos.y;
			is_fade = true;
			break;

		case MAP_2:
			
			curr_map = MAP_1;
			App->fade->FadeToBlack(this, this, 2.0f);
			App->map->CleanUp();
			App->collision->CleanUpMap();
			App->map->Load("Jungle.tmx");
			App->map->setColliders();
			App->player->player_pos.x = App->map->spawn_pos.x;
			App->player->player_pos.y = App->map->spawn_pos.y;
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
			App->player->player_pos.x = App->map->spawn_pos.x;
			App->player->player_pos.y = App->map->spawn_pos.y;
			is_fade = true;
			break;

		case MAP_1:

			curr_map = MAP_2;
			App->fade->FadeToBlack(this, this, 2.0f);
			App->map->CleanUp();
			App->collision->CleanUpMap();
			App->map->Load("Cave.tmx");
			App->map->setColliders();
			App->player->player_pos.x = App->map->spawn_pos.x;
			App->player->player_pos.y = App->map->spawn_pos.y;
			is_fade = true;
			break;
		}
	}

	if (App->player->change_map) {
		App->player->change_map = false;
		switch (curr_map) {
		case MAP_2:

			curr_map = MAP_1;
			App->fade->FadeToBlack(this, this, 2.0f);
			App->map->CleanUp();
			App->collision->CleanUpMap();
			App->map->Load("Jungle.tmx");
			App->map->setColliders();
			App->player->player_pos.x = App->map->spawn_pos.x;
			App->player->player_pos.y = App->map->spawn_pos.y;
			is_fade = true;
			break;

		case MAP_1:

			curr_map = MAP_2;
			App->fade->FadeToBlack(this, this, 2.0f);
			App->map->CleanUp();
			App->collision->CleanUpMap();
			App->map->Load("Cave.tmx");
			App->map->setColliders();
			App->player->player_pos.x = App->map->spawn_pos.x;
			App->player->player_pos.y = App->map->spawn_pos.y;
			is_fade = true;
			break;
		}
	}

	

	if (App->player->player_died) {
		
		App->fade->FadeToBlack(this, this, 2.0f);
	}


	App->map->Draw();
	App->player->Draw();
	
	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
					App->map->data.width, App->map->data.height,
					App->map->data.tile_width, App->map->data.tile_height,
					App->map->data.tilesets.count());

	App->win->SetTitle(title.GetString());
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
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
