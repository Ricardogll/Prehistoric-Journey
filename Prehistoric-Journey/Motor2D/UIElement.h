#ifndef __UIELEMENT_H__
#define __UIELEMENT_H__

#include "j1UI.h"
//#include "SDL/include/SDL_rect.h"
#include "SDL/include/SDL.h"



struct SDL_Texture;


enum class MouseState {

	DOWN_CLICK,
	UP_CLICK,
	REPEAT_CLICK,
	HOVERING,
	ON_ENTER,
	ON_LEAVE,
	NONE

};

class UIElement {

public:

	//SDL_Rect rect;
	UIType type = NONE_UI;
	SDL_Rect rect = { 0,0,0,0 };
	bool clickable = false;
	bool draggable = false;
	MouseState mouse_state = MouseState::NONE;
	MouseState prev_mouse_state = MouseState::NONE;
	iPoint prev_mouse = { 0,0 };
	int local_pos_x = 0;
	int local_pos_y = 0;
	int world_pos_x = 0;
	int world_pos_y = 0;

	SDL_Texture* texture = nullptr;
	SDL_Rect img_rect = { 0,0,0,0 };
	const char* text;
	_TTF_Font* current_font = nullptr;
	UIElement* parent = nullptr;
	iPoint mouse_drag = { 0,0 };



public:
	UIElement() {};
	UIElement(int x, int y, UIType type, UIElement* parent = nullptr);

	virtual ~UIElement();

	virtual void Start();
	virtual void Draw(SDL_Texture* texture);
	virtual void Update();
	MouseState CheckMouseState(int mouse_x, int mouse_y, MouseState mouse_click);
	virtual void SetText(const char * text);



};

#endif //
