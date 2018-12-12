#include "UIImage.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1UI.h"

UIImage::UIImage(int x, int y, SDL_Rect rect, UIElement* parent) :UIElement(x, y, IMAGE, parent)
{
	img_rect = rect;
	this->rect = { world_pos_x,world_pos_y,rect.w,rect.h };
}

UIImage::~UIImage()
{
}

void UIImage::Draw(SDL_Texture* atlas) {

	App->render->Blit(atlas, world_pos_x, world_pos_y, &img_rect);

}