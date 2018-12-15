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
	bool pause = false;
	int score = 100;
	int c_score = 0;
	j1Timer timer;
	bool on_main_menu = true;
	int speed_scroll = -1;
	int limit_map = 0;
	int lifes = 3;
	bool game_saved = false;
private:
	bool is_fade = false;
	UIElement* continue_btn;
	UIElement* settings_btn;
	UIElement* credits_btn;
	UIElement* exit_btn;
	UIElement* play_btn;
	UIElement* menu_title_label;
	UIElement* menu_title_label2;
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
	UIElement* menu_credits_label_authors;
	UIElement* menu_credits_label_license;
	UIElement* menu_credits_back_btn;

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