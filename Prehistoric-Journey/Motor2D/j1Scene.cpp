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

	/*
	
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
	*/


}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	pugi::xml_node variables = config.child("music");

	music_map1 = variables.child("music_map1").attribute("location").as_string();
	music_map2 = variables.child("music_map2").attribute("location").as_string();
	music_main_menu = variables.child("music_main_menu").attribute("location").as_string();
	fx_click_folder = variables.child("fx_click").attribute("location").as_string();
	App->ui->fx_click = App->audio->LoadFx(fx_click_folder.GetString());

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

	if (lifes < 0) {
		lifes = 3;
		on_main_menu = true;
		App->entities->GetPlayer()->state = IDLE;
	}

	
	
	if (App->entities->GetPlayer() == nullptr) {
		if (App->audio->active)
			App->audio->PlayMusic(music_map1.GetString(), 0.5f);			
	}
	else if (App->entities->GetPlayer()->player_died == false) {
		
		App->entities->GetPlayer()->SetMovementZero();
		App->entities->GetPlayer()->state = IDLE;

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
	
	if (on_main_menu)
		App->audio->PlayMusic(music_main_menu.GetString(), 0.5f);

	int w, h;
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
		music_slider_ui = App->ui->CreateSlider(0, 40, App->audio->GetMusicVolume(), 255, { 550,209,222,45 }, { 694,160,37,37 }, music_label_ui);
		App->audio->SetMusicVolume(music_slider_ui->cur_value);
		

		menu_settings_back_btn = App->ui->CreateButton(475, 400, { 550,160,45,49 }, { 595,160,45,49 }, { 640,160,45,49 }, menu_settings);

		menu_credits = App->ui->CreateImage(237, 83, { 0, 0, 549, 474 }, window_ui);
		menu_credits->visible = false;
		menu_credits_back_btn = App->ui->CreateButton(475, 400, { 550,160,45,49 }, { 595,160,45,49 }, { 640,160,45,49 }, menu_credits);

	}
	else {

		//HUD
		switch (lifes)
		{
		case 3:
			life1 = App->ui->CreateImage(15, 10, { 550,254,43,39 }, window_ui);
			life2 = App->ui->CreateImage(65, 10, { 550,254,43,39 }, window_ui);
			life3 = App->ui->CreateImage(115, 10, { 550,254,43,39 }, window_ui);
			break;
		case 2:
			life1 = App->ui->CreateImage(15, 10, { 550,254,43,39 }, window_ui);
			life2 = App->ui->CreateImage(65, 10, { 550,254,43,39 }, window_ui);
			life3 = App->ui->CreateImage(115, 10, { 593,254,43,39 }, window_ui);
			break;
		case 1:
			life1 = App->ui->CreateImage(15, 10, { 550,254,43,39 }, window_ui);
			life2 = App->ui->CreateImage(65, 10, { 593,254,43,39 }, window_ui);
			life3 = App->ui->CreateImage(115, 10, { 593,254,43,39 }, window_ui);
			break;
		default:
			life1 = App->ui->CreateImage(15, 10, { 593,254,43,39 }, window_ui);
			life2 = App->ui->CreateImage(65, 10, { 593,254,43,39 }, window_ui);
			life3 = App->ui->CreateImage(115, 10, { 593,254,43,39 }, window_ui);
			break;
		}

		chickens = App->ui->CreateImage(370, 10, { 636,254,43,39 }, window_ui);
		chickens_numbers = App->ui->CreateLabel(425, 20, "", -1, 24, { 255,255,255,255 }, "fonts/Kenney Future Narrow.ttf", window_ui);

		score_label = App->ui->CreateLabel(520, 20, "Score:", -1, 24, { 255,255,255,255 }, "fonts/Kenney Future Narrow.ttf", window_ui);
		score_numbers = App->ui->CreateLabel(615, 20, "", -1, 24, { 255,255,255,255 }, "fonts/Kenney Future Narrow.ttf", window_ui);

		timer_label = App->ui->CreateLabel(905, 20, "Time:", -1, 24, { 255,255,255,255 }, "fonts/Kenney Future Narrow.ttf", window_ui);
		timer_numbers = App->ui->CreateLabel(975, 20, "", -1, 24, { 255,255,255,255 }, "fonts/Kenney Future Narrow.ttf", window_ui);
	}

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
	
	if (on_main_menu) {
		App->render->camera.x+=speed_scroll;

		if (App->render->camera.x < limit_map)
			speed_scroll = 1;

		if (App->render->camera.x > 0)
			speed_scroll = -1;
	}
	else {

		
		p2SString s_score = std::to_string(score).c_str();
		score_numbers->SetText(s_score.GetString());

		p2SString s_chicken = std::to_string(c_score).c_str();
		chickens_numbers->SetText(s_chicken.GetString());

		p2SString s_time = std::to_string((int)timer.ReadSec()).c_str();
		timer_numbers->SetText(s_time.GetString());
	}
	//if (App->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN) {
	//	App->audio->RaiseVolumeMusic();
	//	App->audio->RaiseVolumeFx();
	//}

	//if (App->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN) {
	//	App->audio->DecreaseVolumeMusic();
	//	App->audio->DecreaseVolumeFx();
	//}

	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) {
		pause = !pause;
	}

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN && App->fade->IsFading()==false) {
		GameLoad();
	}

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN && App->fade->IsFading() == false) {
		GameSave();
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
	
	if (App->fade->IsFading() == false && on_main_menu) {


		if (play_btn->btn_clicked) {
			//menu->visible = false;//Delete UI of main menu
			App->ui->DeleteUIElementChildren(menu);
			App->ui->DeleteUIElementChildren(menu_settings);
			on_main_menu = false;
			App->fade->FadeToBlack(this, this, 2.0f);
			timer.Start();
		}

		if (continue_btn->btn_clicked && game_saved)
		{
			GameLoad();
			App->ui->DeleteUIElementChildren(menu);
			on_main_menu = false;
			App->fade->FadeToBlack(this, this, 2.0f);
			timer.Start();
		}

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
			//ShellExecute(NULL, "open", "https://ricardogll.github.io/Prehistoric-Journey/", NULL, NULL, SW_SHOWNORMAL);
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

		

		if (music_slider_ui->cur_value != music_slider_ui->last_value)
			App->audio->SetMusicVolume(music_slider_ui->cur_value);
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

	if (exit_btn != nullptr) {
		if (exit_btn->btn_clicked)
			ret = false;
	}

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

void j1Scene::GameSave() {
	App->SaveGame();
	saved_score = score;
	saved_c_score = c_score;
	saved_lifes = lifes;
	App->entities->GetPlayer()->saved_map = curr_map;
	game_saved = true;
}

void j1Scene::GameLoad() {

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
	App->entities->GetPlayer()->SetMovementZero();
	
	lifes = saved_lifes;
	score = saved_score;
	c_score = saved_c_score;
}