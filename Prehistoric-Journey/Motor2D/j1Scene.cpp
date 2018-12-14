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
#include "j1UI.h"
#include "UIButton.h"
#include "j1PathFinding.h"
#include "Brofiler/Brofiler.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{

	window_ui->~UIElement();
	window_ui = nullptr;

	continue_btn->~UIElement();
	continue_btn = nullptr;
	settings_btn->~UIElement();
	settings_btn = nullptr;
	credits_btn->~UIElement();
	credits_btn = nullptr;
	exit_btn->~UIElement();
	exit_btn = nullptr;
	play_btn->~UIElement();
	play_btn = nullptr;

	menu_title_label->~UIElement();
	menu_title_label = nullptr;
	menu_continue_label->~UIElement();
	menu_continue_label = nullptr;
	menu_settings_label->~UIElement();
	menu_settings_label = nullptr;
	menu_credits_label->~UIElement();
	menu_credits_label = nullptr;
	menu_exit_label->~UIElement();
	menu_exit_label = nullptr;
	menu_play_label->~UIElement();
	menu_play_label = nullptr;

	menu->~UIElement();
	menu = nullptr;


}

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

	int w, h;//change to differentiate between maps
	uchar* data = NULL;
	if (App->map->CreateWalkabilityMap(w, h, &data))
		App->pathfinding->SetMap(w, h, data);

	RELEASE_ARRAY(data);

	window_ui = new UIElement(App->render->camera.x, 0, WINDOW);
	window_ui->visible = true;

	if (on_main_menu) {
		menu = App->ui->CreateImage(237, 83, { 0, 0, 549, 474 }, window_ui);
		//menu->draggable = true;
		menu_title_label = App->ui->CreateLabel(85, 50, "abc de f ghi jk lmn op q rst uvwxyz", 250, 32, { 255,0,0,255 }, "fonts/Kenney Future Narrow.ttf", menu);

		continue_btn = App->ui->CreateButton(50, 140, { 550,0,190,49 }, { 550,49,190,49 }, { 550,98,190,49 }, menu);
		menu_continue_label = App->ui->CreateLabel(55, 13, "Continue", -1, 16, { 255,0,0,255 }, "fonts/Kenney Future Narrow.ttf", continue_btn);

		settings_btn = App->ui->CreateButton(305, 140, { 550,0,190,49 }, { 550,49,190,49 }, { 550,98,190,49 }, menu);
		menu_settings_label = App->ui->CreateLabel(55, 13, "Settings", -1, 16, { 255,0,0,255 }, "fonts/Kenney Future Narrow.ttf", settings_btn);

		credits_btn = App->ui->CreateButton(50, 240, { 550,0,190,49 }, { 550,49,190,49 }, { 550,98,190,49 }, menu);
		menu_credits_label = App->ui->CreateLabel(55, 13, "Credits", -1, 16, { 255,0,0,255 }, "fonts/Kenney Future Narrow.ttf", credits_btn);

		exit_btn = App->ui->CreateButton(305, 240, { 550,0,190,49 }, { 550,49,190,49 }, { 550,98,190,49 }, menu);
		menu_exit_label = App->ui->CreateLabel(80, 13, "Exit", -1, 16, { 255,0,0,255 }, "fonts/Kenney Future Narrow.ttf", exit_btn);

		play_btn = App->ui->CreateButton(180, 350, { 550,0,190,49 }, { 550,49,190,49 }, { 550,98,190,49 }, menu);
		menu_play_label = App->ui->CreateLabel(75, 13, "Play", -1, 16, { 255,0,0,255 }, "fonts/Kenney Future Narrow.ttf", play_btn);

		menu_settings = App->ui->CreateImage(237, 83, { 0, 0, 549, 474 }, window_ui);
		menu_settings->visible = false;
		music_label_ui = App->ui->CreateLabel(50, 75, "Music", -1, 24, { 255,255,255,255 }, "fonts/Kenney Future Narrow.ttf", menu_settings);
		music_slider_ui = App->ui->CreateSlider(0, 40, 255, { 550,209,222,45 }, { 694,160,37,37 }, music_label_ui);

		menu_settings_back_btn = App->ui->CreateButton(475, 400, { 550,160,45,49 }, { 595,160,45,49 }, { 640,160,45,49 }, menu_settings);

		menu_credits = App->ui->CreateImage(237, 83, { 0, 0, 549, 474 }, window_ui);
		menu_credits->visible = false;
		menu_credits_back_btn = App->ui->CreateButton(475, 400, { 550,160,45,49 }, { 595,160,45,49 }, { 640,160,45,49 }, menu_credits);

	}

	//HUD
	switch (App->entities->lifes)
	{
	case 3:
		life1 = App->ui->CreateImage( 15, 10, { 550,254,43,39 }, window_ui);
		life2 = App->ui->CreateImage( 65, 10, { 550,254,43,39 }, window_ui);
		life3 = App->ui->CreateImage(115, 10, { 550,254,43,39 }, window_ui);
		break;
	case 2:
		life1 = App->ui->CreateImage( 15, 10, { 550,254,43,39 }, window_ui);
		life2 = App->ui->CreateImage( 65, 10, { 550,254,43,39 }, window_ui);
		life3 = App->ui->CreateImage(115, 10, { 593,254,43,39 }, window_ui);
		break;
	case 1:
		life1 = App->ui->CreateImage( 15, 10, { 550,254,43,39 }, window_ui);
		life2 = App->ui->CreateImage( 65, 10, { 593,254,43,39 }, window_ui);
		life3 = App->ui->CreateImage(115, 10, { 593,254,43,39 }, window_ui);
		break;
	}

	chickens = App->ui->CreateImage(370, 10, { 636,254,43,39 }, window_ui);
	chickens_numbers = App->ui->CreateLabel(425, 20, "", -1, 24, { 255,255,255,255 }, "fonts/Kenney Future Narrow.ttf", window_ui);

	score_label = App->ui->CreateLabel(520, 20, "Score:", -1, 24, { 255,255,255,255 }, "fonts/Kenney Future Narrow.ttf", window_ui);
	score_numbers = App->ui->CreateLabel(615, 20, "", -1, 24, { 255,255,255,255 }, "fonts/Kenney Future Narrow.ttf", window_ui);

	timer_label = App->ui->CreateLabel(905, 20, "Time:", -1, 24, { 255,255,255,255 }, "fonts/Kenney Future Narrow.ttf", window_ui);
	timer_numbers = App->ui->CreateLabel(975, 20, "", -1, 24, { 255,255,255,255 }, "fonts/Kenney Future Narrow.ttf", window_ui);

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

	//For debug only (for now)
	if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN) {
		score += 50;
		c_score += 2;
		
	}
	
	if (on_main_menu)
		App->render->camera.x--;


	
	//std::string s = std::to_string(score);
	//p2SString s2 = s.c_str();
	//App->font->CalcSize(s2.GetString(), score_numbers->img_rect.w, score_numbers->img_rect.h, App->font->default);
	//score_numbers->UpdateText(App->font->Print(s2.GetString(), { 255,255,255,255 }, App->font->default));		//CHANGE THIS TO ...
	p2SString s2 = std::to_string(score).c_str(); // THIS
	score_numbers->SetText(s2.GetString());

	std::string s3 = std::to_string(c_score);
	p2SString s4 = s3.c_str();
	App->font->CalcSize(s4.GetString(), chickens_numbers->img_rect.w, chickens_numbers->img_rect.h, App->font->default);
	chickens_numbers->UpdateText(App->font->Print(s4.GetString(), { 255,255,255,255 }, App->font->default));

	std::string s5 = std::to_string((int)timer.ReadSec());
	p2SString s6 = s5.c_str();
	App->font->CalcSize(s6.GetString(), timer_numbers->img_rect.w, timer_numbers->img_rect.h, App->font->default);
	timer_numbers->UpdateText(App->font->Print(s6.GetString(), { 255,255,255,255 }, App->font->default));

	if (App->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN) {
		App->audio->RaiseVolumeMusic();
		App->audio->RaiseVolumeFx();
	}

	if (App->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN) {
		App->audio->DecreaseVolumeMusic();
		App->audio->DecreaseVolumeFx();
	}

	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) {
		pause = !pause;
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
				//App->entities->GetPlayer()->position.x = App->entities->GetPlayer()->last_saved_pos.x;
				//App->entities->GetPlayer()->position.y = App->entities->GetPlayer()->last_saved_pos.y;
				is_fade = true;
				break;
			case 2:
				curr_map = MAP_2;
				App->fade->FadeToBlack(this, this, 2.0f);
				App->map->CleanUp();
				App->collision->CleanUpMap();
				App->map->Load("Cave.tmx");
				App->map->setColliders();
				//App->entities->GetPlayer()->position.x = App->entities->GetPlayer()->last_saved_pos.x;
				//App->entities->GetPlayer()->position.y = App->entities->GetPlayer()->last_saved_pos.y;
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
			App->entities->GetPlayer()->position.x = App->map->spawn_pos.x;
			App->entities->GetPlayer()->position.y = App->map->spawn_pos.y;
			is_fade = true;
			break;

		case MAP_2:
			
			curr_map = MAP_1;
			App->fade->FadeToBlack(this, this, 2.0f);
			App->map->CleanUp();
			App->collision->CleanUpMap();
			App->map->Load("Jungle.tmx");
			App->map->setColliders();
			App->entities->GetPlayer()->position.x = App->map->spawn_pos.x;
			App->entities->GetPlayer()->position.y = App->map->spawn_pos.y;
			is_fade = true;
			break;
		}
		App->entities->DeleteEnemies();
		App->entities->SetEnemies(1);
		
	}

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN && App->fade->IsFading() == false)
	{
		switch (curr_map) {
		case MAP_2:

			curr_map = MAP_2;
			App->fade->FadeToBlack(this, this, 2.0f);
			App->entities->GetPlayer()->position.x = App->map->spawn_pos.x;
			App->entities->GetPlayer()->position.y = App->map->spawn_pos.y;
			is_fade = true;
			break;

		case MAP_1:

			curr_map = MAP_2;
			App->fade->FadeToBlack(this, this, 2.0f);
			App->map->CleanUp();
			App->collision->CleanUpMap();
			App->map->Load("Cave.tmx");
			App->map->setColliders();
			App->entities->GetPlayer()->position.x = App->map->spawn_pos.x;
			App->entities->GetPlayer()->position.y = App->map->spawn_pos.y;
			is_fade = true;
			break;
		}
		App->entities->DeleteEnemies();
		App->entities->SetEnemies(2);
	}

	if (App->entities->GetPlayer()->change_map) {
		App->entities->GetPlayer()->change_map = false;
		App->entities->DeleteEnemies();
		switch (curr_map) {
		case MAP_2:

			curr_map = MAP_1;
			App->fade->FadeToBlack(this, this, 2.0f);
			App->map->CleanUp();
			App->collision->CleanUpMap();
			App->map->Load("Jungle.tmx");
			App->map->setColliders();
			App->entities->GetPlayer()->position.x = App->map->spawn_pos.x;
			App->entities->GetPlayer()->position.y = App->map->spawn_pos.y;
			is_fade = true;
			App->entities->SetEnemies(1);
			break;

		case MAP_1:

			curr_map = MAP_2;
			App->fade->FadeToBlack(this, this, 2.0f);
			App->map->CleanUp();
			App->collision->CleanUpMap();
			App->map->Load("Cave.tmx");
			App->map->setColliders();
			App->entities->GetPlayer()->position.x = App->map->spawn_pos.x;
			App->entities->GetPlayer()->position.y = App->map->spawn_pos.y;
			is_fade = true;
			App->entities->SetEnemies(2);
			break;
		}
	}

	

	if (App->entities->GetPlayer()->player_died) {
		
		App->entities->DeleteEnemies();
		if(curr_map==MAP_1)
			App->entities->SetEnemies(1);
		else
			App->entities->SetEnemies(2);


		App->fade->FadeToBlack(this, this, 2.0f);
	}

	//UI
	window_ui->world_pos_x = -App->render->camera.x;
	
	if (settings_btn->btn_clicked)
	{
		
		menu_settings->visible = true;
		music_label_ui->visible = true;
		music_slider_ui->visible = true;
		menu_settings_back_btn->visible = true;
		menu->visible = false;
	}

	if (menu_settings_back_btn->btn_clicked)
	{
		menu->visible = true;
		continue_btn->visible = true;
		settings_btn->visible = true;
		credits_btn->visible = true;
		exit_btn->visible = true;
		play_btn->visible = true;
		menu_title_label->visible = true;
		menu_continue_label->visible = true;
		menu_settings_label->visible = true;
		menu_credits_label->visible = true;
		menu_exit_label->visible = true;
		menu_play_label->visible = true;
		menu_settings->visible = false;
	}

	if (credits_btn->btn_clicked)
	{

		menu_credits->visible = true;
		menu_credits_back_btn->visible = true;
		menu->visible = false;
	}

	if (menu_credits_back_btn->btn_clicked)
	{
		menu->visible = true;
		continue_btn->visible = true;
		settings_btn->visible = true;
		credits_btn->visible = true;
		exit_btn->visible = true;
		play_btn->visible = true;
		menu_title_label->visible = true;
		menu_continue_label->visible = true;
		menu_settings_label->visible = true;
		menu_credits_label->visible = true;
		menu_exit_label->visible = true;
		menu_play_label->visible = true;
		menu_credits->visible = false;
	}

	if (continue_btn->btn_clicked) {
		menu->visible = false;//Delete UI of main menu
		on_main_menu = false;
	}

	App->map->Draw();
	
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	BROFILER_CATEGORY("PostUpdate Scene", Profiler::Color::RoyalBlue)
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || exit_btn->btn_clicked)
		ret = false;


	//Draw pathfinding
	if (App->collision->debug) {
		const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();
		int id = (curr_map == MAP_2) ? 211 : 61;
		int tileset = (curr_map == MAP_2) ? 1 : 0;
		if (path) {
			for (uint i = 0; i < path->Count(); ++i)
			{
				iPoint pos = App->map->MapToWorld(path->At(i)->x, path->At(i)->y);
				App->render->Blit(App->map->data.tilesets.At(tileset)->data->texture, pos.x, pos.y, &App->map->data.tilesets.At(tileset)->data->GetTileRect(id));

			}
		}
	}


	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	App->ui->DeleteUIElements();

	return true;
}
