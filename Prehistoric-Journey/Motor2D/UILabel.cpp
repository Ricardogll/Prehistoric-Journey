#include "UILabel.h"
#include "j1Fonts.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Audio.h"

UILabel::UILabel(int x, int y, const char* text, int width, int size,  SDL_Color color, const char* font, UIElement* parent) :UIElement(x, y, LABEL, parent)
{

	this->text = text;
	this->color = color;


	this->rect = { world_pos_x,world_pos_y,0,0 };

	_TTF_Font* newFont = App->font->Load(font, size);
	current_font = newFont;
	App->font->CalcSize(text, rect.w, rect.h, newFont);

	if(width < 0)
		texture = App->font->Print(text, color, newFont);
	else
		texture = App->font->PrintMultiLine(text, width, color, newFont);


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