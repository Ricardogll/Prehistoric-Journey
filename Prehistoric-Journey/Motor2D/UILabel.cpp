#include "UILabel.h"
#include "j1Fonts.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"


UILabel::UILabel(int x, int y, const char* text, int size, SDL_Color color, const char* font) :UIElement(x, y, LABEL)
{

	this->text = text;
	this->color = color;
	rect.x = world_pos_x;
	rect.y = world_pos_y;


	_TTF_Font* newFont = App->font->Load(font, size);
	current_font = newFont;//Check if this pointer works alright
	App->font->CalcSize(text, rect.w, rect.h, newFont);

	texture = App->font->Print(text, color, newFont);

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