#include "UISlider.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1UI.h"
#include "j1Input.h"
#include "p2Log.h"



UISlider::UISlider(int x, int y, float max_value, SDL_Rect bar, SDL_Rect ball, UIElement* parent):UIElement(x,y,SLIDER,parent)
{
	clickable = true;
	draggable = true;
	//this->rect = { world_pos_x,world_pos_y,bar.w,bar.h };
	bar_rect = bar;
	ball_rect = ball;
	world_ball_x = world_pos_x;
	world_ball_y = world_pos_y + bar_rect.h / 2 - ball_rect.h / 2;
	this->rect = { world_ball_x,world_ball_y,ball_rect.w,ball_rect.h };
	this->max_value = max_value;
	SetSliderValue(0.5,1);
}

UISlider::~UISlider() {

}


void UISlider::Draw(SDL_Texture* atlas) {

	App->render->Blit(atlas, world_pos_x, world_pos_y, &bar_rect);
	App->render->Blit(atlas, world_ball_x, world_ball_y, &ball_rect);
}


void UISlider::Update() {

	iPoint mouse_pos_aux;
	App->input->GetMousePosition(mouse_pos_aux.x, mouse_pos_aux.y);


	if (mouse_state == MouseState::REPEAT_CLICK && draggable) {

		if (prev_mouse != mouse_pos_aux) {
			//iPoint mouse_motion;
			//App->input->GetMouseMotion(mouse_motion.x, mouse_motion.y);
			local_ball_x += mouse_pos_aux.x - prev_mouse.x;
			
			//world_ball_x += mouse_pos_aux.x - prev_mouse.x;
			
			//LOG("MouseMotion: %i, %i", mouse_motion.x, mouse_motion.y);
		}

	}


	if (parent == nullptr) {//maybe more useful the other way around? check when more developed
		world_pos_x = local_pos_x;
		world_pos_y = local_pos_y;
	}
	else
	{

		world_pos_x = local_pos_x + parent->world_pos_x;
		world_pos_y = local_pos_y + parent->world_pos_y;
	}

	if (local_ball_x > bar_rect.w - ball_rect.w)
		local_ball_x = bar_rect.w - ball_rect.w;
	else if (local_ball_x < 0)
		local_ball_x = 0;

	//rect.x = world_pos_x;
	//rect.y = world_pos_y;

	world_ball_x = local_ball_x + world_pos_x;
	world_ball_y = local_ball_y + world_pos_y;
	
	rect.x = world_ball_x;
	rect.y = world_ball_y;

	prev_mouse = mouse_pos_aux;

	if (parent != nullptr) {
		if (!parent->visible)
			visible = false;
	}



	GetSliderValue();
}


float UISlider::GetSliderValue() const{

	if (bar_rect.w-ball_rect.w != 0) {
		float value = ((float)world_ball_x - (float)world_pos_x) / ((float)bar_rect.w - (float)ball_rect.w); //value between 0 and 1 

		LOG("Slider value: %f", value * max_value);
		return value * max_value;
	}
	
	return 0;
}


void UISlider::SetSliderValue(float value, float max_value) {

	local_ball_x = (value / max_value) * (bar_rect.w - ball_rect.w);

}