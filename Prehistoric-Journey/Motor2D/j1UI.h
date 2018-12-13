#ifndef __j1UI_H__
#define __j1UI_H__

#include "j1Module.h"
#include "p2DynArray.h"
#include "j1Fonts.h"
#include "p2SString.h"
#include "j1Render.h"

#define CURSOR_WIDTH 2

// TODO 1: Create your structure of classes



// ---------------------------------------------------
enum UIType {

	LABEL,
	BUTTON,
	IMAGE,
	WINDOW,
	SLIDER,
	NONE_UI

};

class UIElement;

class j1UI : public j1Module
{

public:

	j1UI();

	// Destructor
	virtual ~j1UI();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	bool Update(float dt);

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void DeleteUIElements();

	// Gui creation functions
	UIElement* CreateLabel(int x, int y, const char* text, int size = DEFAULT_FONT_SIZE, SDL_Color color = { 255,255,255,255 }, const char* font = DEFAULT_FONT, UIElement* parent = nullptr);
	UIElement* CreateImage(int x, int y, SDL_Rect rect, UIElement* parent = nullptr);
	//UIElement* CreateImageNoAtlas(int x, int y, SDL_Rect rect, SDL_Texture* tex);
	UIElement* CreateButton(int x, int y, SDL_Rect rect, SDL_Rect hovering, SDL_Rect clicked, UIElement* parent = nullptr);
	UIElement* CreateSlider(int x, int y, SDL_Rect bar, SDL_Rect ball, UIElement* parent = nullptr);

	const SDL_Texture* GetAtlas() const;


public:

	

private:
	bool debug = false;

	p2DynArray<UIElement*> ui_elements;
	SDL_Texture* atlas;

	p2SString atlas_file_name;
};

#endif //