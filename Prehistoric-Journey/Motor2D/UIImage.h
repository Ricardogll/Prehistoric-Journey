#ifndef __UIIMAGE__
#define __UIIMAGE__

#include "UIElement.h"


class UIImage : public UIElement {

public:
	UIImage(int x, int y, SDL_Rect rect, UIElement* parent);

	void Draw(SDL_Texture* atlas);

private:


};

#endif // !_UILABEL__
