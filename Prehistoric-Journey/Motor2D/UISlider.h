#ifndef __UISLIDER__
#define __UISLIDER__

#include "UIElement.h"


class UISlider : public UIElement {

public:
	UISlider(int x, int y, SDL_Rect bar, SDL_Rect ball, UIElement* parent);
	~UISlider();
	void Draw(SDL_Texture* atlas);
	void Update();
	float GetSliderValue(int max_value) const;

private:
	int world_ball_x = 0;
	int world_ball_y = 0;
	int local_ball_x = 0;
	int local_ball_y = 0;

	SDL_Rect bar_rect;
	SDL_Rect ball_rect;
	
};

#endif 
