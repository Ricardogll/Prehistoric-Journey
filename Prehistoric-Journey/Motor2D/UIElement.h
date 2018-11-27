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
	HOVER,
	NONE

};

class UIElement {

public:

	SDL_Rect rect;
	UIType type;
	bool clickable;
	MouseState mouse_state;
	SDL_Rect sprite_rect;
	SDL_Texture* texture;
	p2SString text;




public:
	UIElement() {};
	UIElement(int x, int y, UIType type, SDL_Texture* texture, bool clickable, p2SString &str) {};

	virtual ~UIElement();

	virtual void Start();
	virtual void Draw();
	virtual void Update();
	virtual void CheckMouseState();


};

#endif // !__UIELEMENT_H__
