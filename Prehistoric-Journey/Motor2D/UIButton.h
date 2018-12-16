#ifndef __UIBUTTON__
#define __UIBUTTON__

#include "UIElement.h"


class UIButton : public UIElement {

public:
	UIButton(int x, int y, SDL_Rect rect, SDL_Rect hovering, SDL_Rect clicked, UIElement* parent);
	
	void Draw(SDL_Texture* atlas);
	void Update();


private:
	SDL_Rect basic_rect;
	SDL_Rect clicked_rect;
	SDL_Rect hovering_rect;
};

#endif 
