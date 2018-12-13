#include "UILabel.h"
#include "j1Fonts.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"


UILabel::UILabel(int x, int y, const char* text, int size, SDL_Color color, const char* font, UIElement* parent) :UIElement(x, y, LABEL, parent)
{

	this->text = text;
	this->color = color;

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

	this->rect = { world_pos_x,world_pos_y,0,0 };

	_TTF_Font* newFont = App->font->Load(font, size);
	current_font = newFont;
	App->font->CalcSize(text, rect.w, rect.h, newFont);

	texture = App->font->Print(text, color, newFont);

	/*App->font->Unload(newFont);
	newFont = nullptr;*/
}


UILabel::~UILabel()
{
}




void UILabel::Draw(SDL_Texture* atlas) {

	App->render->Blit(texture, world_pos_x, world_pos_y);
	

}

void UILabel::SetText(const char* text) {

	this->text = text;
	App->font->CalcSize(text, rect.w, rect.h, current_font);
	App->tex->UnLoad(texture);
	texture = App->font->Print(text, color, current_font);

}