#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

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

public:
	CurrentMap curr_map = NO_MAP;
	p2SString music_map1;
	p2SString music_map2;
	
private:
	bool is_fade = false;
	UIElement* test_ui;
	UIElement* ui_el;
	UIElement* menu;
	UIElement* window_ui;
	UIElement* slider_ui;
	

};

#endif // __j1SCENE_H__