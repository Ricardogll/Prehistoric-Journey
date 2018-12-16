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
	int score = 0;
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
	int score_2nd_lvl = 0;
	int c_score_2nd_lvl = 0;
	float time_2nd_lvl = 0.0f;
	int multiplier = 1;
	float saved_time = 0.0f;
private:
	bool is_fade = false;
	UIElement* continue_btn=nullptr;
	UIElement* settings_btn = nullptr;
	UIElement* credits_btn = nullptr;
	UIElement* exit_btn = nullptr;
	UIElement* play_btn = nullptr;
	UIElement* menu_title_label = nullptr;
	UIElement* menu_continue_label = nullptr;
	UIElement* menu_settings_label = nullptr;
	UIElement* menu_credits_label = nullptr;
	UIElement* menu_exit_label = nullptr;
	UIElement* menu_play_label = nullptr;
	UIElement* menu = nullptr;
	UIElement* window_ui = nullptr;

	UIElement* menu_settings = nullptr;
	UIElement* music_slider_ui = nullptr;
	UIElement* music_label_ui = nullptr;
	UIElement* fx_slider_ui = nullptr;
	UIElement* fx_label_ui = nullptr;
	UIElement* menu_settings_back_btn = nullptr;

	UIElement* menu_credits = nullptr;
	UIElement* menu_credits_authors = nullptr;
	UIElement* menu_credits_license = nullptr;
	UIElement* menu_credits_back_btn = nullptr;
	UIElement* menu_credits_web_btn = nullptr;

	UIElement* menu_in_game = nullptr;
	UIElement* menu_in_game_label = nullptr;
	UIElement* menu_in_game_settings_label = nullptr;
	UIElement* menu_in_game_settings_btn = nullptr;
	UIElement* menu_in_game_resume_label = nullptr;
	UIElement* menu_in_game_resume_btn = nullptr;
	UIElement* menu_in_game_back_label = nullptr;
	UIElement* menu_in_game_back_btn = nullptr;

	UIElement* menu_in_game_settings = nullptr;
	UIElement* music_in_game_slider_ui = nullptr;
	UIElement* music_in_game_label_ui = nullptr;
	UIElement* fx_in_game_slider_ui = nullptr;
	UIElement* fx_in_game_label_ui = nullptr;
	UIElement* menu_in_game_settings_back_btn = nullptr;

	UIElement* life1 = nullptr;
	UIElement* life2 = nullptr;
	UIElement* life3 = nullptr;

	UIElement* chickens = nullptr;
	UIElement* chickens_numbers = nullptr;

	UIElement* score_label = nullptr;
	UIElement* score_numbers = nullptr;

	UIElement* timer_label = nullptr;
	UIElement* timer_numbers = nullptr;
};

#endif // __j1SCENE_H__