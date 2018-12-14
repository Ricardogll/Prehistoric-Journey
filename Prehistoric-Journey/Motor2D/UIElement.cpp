#include "UIElement.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "p2Log.h"
#include "j1Render.h"

UIElement::UIElement(int x, int y, UIType type, UIElement* parent) : local_pos_x(x), local_pos_y(y), type(type), parent(parent)
{
	
	clickable = true;
	visible = true;
	

	if (parent != nullptr)
	{
		world_pos_x = parent->local_pos_x + x;
		world_pos_y = parent->local_pos_y + y;	
	}
	else
	{
		world_pos_x = x;
		world_pos_y = y;
	}

	
}

UIElement::~UIElement()
{
	App->tex->UnLoad(texture);
	texture = nullptr;
	parent = nullptr;
	//App->font->Unload(current_font);
	current_font = nullptr;
}

void UIElement::Start()
{
}

void UIElement::Draw(SDL_Texture* texture)
{
}

void UIElement::Update()
{
	

	SetPositionWithParent();

}

void UIElement::SetPositionWithParent() {

	iPoint mouse_pos_aux;
	App->input->GetMousePosition(mouse_pos_aux.x, mouse_pos_aux.y);


	if (mouse_state == MouseState::REPEAT_CLICK && draggable) {

		if (prev_mouse != mouse_pos_aux) {
			//iPoint mouse_motion;
			//App->input->GetMouseMotion(mouse_motion.x, mouse_motion.y);
			local_pos_x += mouse_pos_aux.x - prev_mouse.x;
			local_pos_y += mouse_pos_aux.y - prev_mouse.y;
			
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


	rect.x = world_pos_x;
	rect.y = world_pos_y;

	prev_mouse = mouse_pos_aux;

	if (parent != nullptr) {
		if (!parent->visible)
			visible = false;
	}
}


void UIElement::CheckMouseState(int mouse_x, int mouse_y, MouseState mouse_click)
{
	mouse_state = MouseState::NONE;

	if (clickable)
	{
		bool mouse_inside_rect = false;

		if (rect.x <= mouse_x && mouse_x <= rect.x + rect.w && rect.y <= mouse_y && mouse_y <= rect.y + rect.h) {
			//mouse inside rect
			mouse_inside_rect = true;


			switch (mouse_click)
			{
			case MouseState::DOWN_CLICK:
				mouse_state = MouseState::DOWN_CLICK;
				//LOG("DOWNCLICK");

				App->input->GetMousePosition(mouse_drag.x, mouse_drag.y);

				break;

			case MouseState::REPEAT_CLICK:
				mouse_state = MouseState::REPEAT_CLICK;
				//LOG("REPEATCLICK");
				break;

			case MouseState::UP_CLICK:
				mouse_state = MouseState::UP_CLICK;
				//LOG("UPCLICK");
				break;

			default:
				mouse_state = MouseState::HOVERING;
				//LOG("HOVERING");
				break;
			}


			if (prev_mouse_state == MouseState::ON_ENTER) {
				mouse_state = MouseState::HOVERING;
				//LOG("HOVERING AFTER ON ENTER");
			}

			if (prev_mouse_state == MouseState::NONE) {
				mouse_state = MouseState::ON_ENTER;
				//LOG("ON ENTER");
			}
		}else if (prev_mouse_state == MouseState::REPEAT_CLICK) {
			if (mouse_click == MouseState::REPEAT_CLICK) {
				mouse_state = MouseState::REPEAT_CLICK;
			}if (mouse_click == MouseState::UP_CLICK)
				mouse_state = MouseState::UP_CLICK;
		}

		if ((prev_mouse_state == MouseState::HOVERING || prev_mouse_state == MouseState::ON_ENTER) && !mouse_inside_rect) {
			mouse_state = MouseState::ON_LEAVE;
			//LOG("ON LEAVE");
		}
	}
	prev_mouse_state = mouse_state;

	//return mouse_state;
}

void UIElement::UpdateText(SDL_Texture* tex)
{
	if (texture != nullptr)
		App->tex->UnLoad((SDL_Texture*)texture);

	texture = tex;
}

void UIElement::SetText(const char * text)
{

}


