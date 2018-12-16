#include "UIButton.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1UI.h"
#include "j1Audio.h"

//#include "p2Log.h"

UIButton::UIButton(int x, int y, SDL_Rect rect, SDL_Rect hovering, SDL_Rect clicked, UIElement* parent) :UIElement(x, y, BUTTON, parent)
{
	img_rect = rect;
	basic_rect = rect;
	hovering_rect = hovering;
	clicked_rect = clicked;

	this->rect = { world_pos_x,world_pos_y,rect.w,rect.h };

}



void UIButton::Update() {

	btn_clicked = false;

	switch (mouse_state) {

	case MouseState::DOWN_CLICK:
		App->audio->PlayFx(App->ui->fx_click);
	case MouseState::REPEAT_CLICK:
		img_rect = clicked_rect;
		break;

	case MouseState::HOVERING:
		img_rect = hovering_rect;
		break;
	case MouseState::UP_CLICK:
		btn_clicked = true;
		break;
	default:
		img_rect = basic_rect;
		break;
	}



	

	SetPositionWithParent();

}

void UIButton::Draw(SDL_Texture* atlas) {

	App->render->Blit(atlas, world_pos_x, world_pos_y, &img_rect);

}

