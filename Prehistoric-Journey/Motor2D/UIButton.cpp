#include "UIButton.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1UI.h"
//#include "p2Log.h"

UIButton::UIButton(int x, int y, SDL_Rect rect, SDL_Rect hovering, SDL_Rect clicked, UIElement* parent) :UIElement(x, y, BUTTON, parent)
{
	img_rect = rect;
	basic_rect = rect;
	hovering_rect = hovering;
	clicked_rect = clicked;
	//if (parent != nullptr)
	//{
	//	local_pos_x = parent->local_pos_x + x;
	//	local_pos_y = parent->local_pos_y + y;
	//}
	//else
	//{
	//	local_pos_x = x;
	//	local_pos_y = y;
	//}
	this->rect = { world_pos_x,world_pos_y,rect.w,rect.h };

}

UIButton::~UIButton()
{
}

void UIButton::Update() {

	btn_clicked = false;

	switch (mouse_state) {

	case MouseState::DOWN_CLICK:
	case MouseState::REPEAT_CLICK:
		img_rect = clicked_rect;
		break;

	case MouseState::HOVERING:
		img_rect = hovering_rect;
		break;

	default:
		img_rect = basic_rect;
		break;
	}



	if (mouse_state == MouseState::DOWN_CLICK)
		btn_clicked = true;


	SetPositionWithParent();

}

void UIButton::Draw(SDL_Texture* atlas) {

	App->render->Blit(atlas, world_pos_x, world_pos_y, &img_rect);

}

