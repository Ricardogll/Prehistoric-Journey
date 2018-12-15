#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "j1Timer.h"

struct SDL_Texture;
class UIElement;

enum CurrentMap
{
	NO_MAP = 0,
	MAP_1,
	MAP_2,
};

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void GameSave();
	void GameLoad();

public:
	CurrentMap curr_map = NO_MAP;
	p2SString music_map1;
	p2SString music_map2;
	p2SString music_main_menu;
	p2SString fx_click_folder;
	bool pause = false;
	int score = 100;
	int c_score = 0;
	j1Timer timer;
	float time = 0.0f;
	bool on_main_menu = true;
	bool on_pause_menu = false;
	int speed_scroll = -1;
	int limit_map = 0;
	int lifes = 3;
	bool game_saved = false;
	int saved_score = 0;
	int saved_c_score = 0;
	int saved_lifes = 0;
	float saved_time = 0.0f;
private:
	bool is_fade = false;
	UIElement* continue_btn;
	UIElement* settings_btn;
	UIElement* credits_btn;
	UIElement* exit_btn;
	UIElement* play_btn;
	UIElement* menu_title_label;
	UIElement* menu_continue_label;
	UIElement* menu_settings_label;
	UIElement* menu_credits_label;
	UIElement* menu_exit_label;
	UIElement* menu_play_label;   
	UIElement* menu;
	UIElement* window_ui;	

	UIElement* menu_settings;
	UIElement* music_slider_ui;
	UIElement* music_label_ui;
	UIElement* fx_slider_ui;
	UIElement* fx_label_ui;
	UIElement* menu_settings_back_btn;

	UIElement* menu_credits;
	UIElement* menu_credits_authors;
	UIElement* menu_credits_license;
	UIElement* menu_credits_back_btn;

	UIElement* menu_in_game;
	UIElement* menu_in_game_label;
	UIElement* menu_in_game_settings_label;
	UIElement* menu_in_game_settings_btn;
	UIElement* menu_in_game_resume_label;
	UIElement* menu_in_game_resume_btn;
	UIElement* menu_in_game_back_label;
	UIElement* menu_in_game_back_btn;

	UIElement* menu_in_game_settings;
	UIElement* music_in_game_slider_ui;
	UIElement* music_in_game_label_ui;
	UIElement* fx_in_game_slider_ui;
	UIElement* fx_in_game_label_ui;
	UIElement* menu_in_game_settings_back_btn;

	UIElement* life1;
	UIElement* life2;
	UIElement* life3;

	UIElement* chickens;
	UIElement* chickens_numbers;

	UIElement* score_label;
	UIElement* score_numbers;

	UIElement* timer_label;
	UIElement* timer_numbers;
};

#endif // __j1SCENE_H__