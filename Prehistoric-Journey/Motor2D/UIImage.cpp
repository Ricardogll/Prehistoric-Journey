#include "UIImage.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1UI.h"

UIImage::UIImage(int x, int y, SDL_Rect rect, UIElement* parent) :UIElement(x, y, IMAGE, parent)
{
	img_rect = rect;

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



void UIImage::Draw(SDL_Texture* atlas) {

	App->render->Blit(atlas, world_pos_x, world_pos_y, &img_rect);

}