#ifndef __UILABEL__
#define __UILABEL__

#include "UIElement.h"


class UILabel : public UIElement {

public:
	UILabel(int x, int y, const char* text, int size, SDL_Color color, const char* font);
	~UILabel();

	void SetText(const char* text);

private:

	void Draw(SDL_Texture* atlas);


public:
	SDL_Color color;

};

#endif // !_UILABEL__
