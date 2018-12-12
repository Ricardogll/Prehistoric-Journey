#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1UI.h"
#include "UIImage.h"
#include "UILabel.h"
#include "UIButton.h"



j1UI::j1UI() : j1Module()
{
	name.create("gui");
}

// Destructor
j1UI::~j1UI()
{}

// Called before render is available
bool j1UI::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1UI::Start()
{
	atlas = App->tex->Load(atlas_file_name.GetString());
	

	return true;
}

// Update all guis
bool j1UI::PreUpdate()
{
	return true;
}

bool j1UI::Update(float dt)
{
	int mouse_x = 0, mouse_y = 0;
	MouseState mouse_click = MouseState::NONE;

	switch (App->input->GetMouseButtonDown(1)) {

	case j1KeyState::KEY_DOWN:
		mouse_click = MouseState::DOWN_CLICK;
		break;

	case j1KeyState::KEY_REPEAT:
		mouse_click = MouseState::REPEAT_CLICK;
		break;

	case j1KeyState::KEY_UP:
		mouse_click = MouseState::UP_CLICK;
		break;

	}

	App->input->GetMousePosition(mouse_x, mouse_y);

	for (uint i = 0u; i < ui_elements.Count(); i++) {
		if (ui_elements[i] != nullptr) {
			ui_elements[i]->CheckMouseState(mouse_x, mouse_y, mouse_click);
			ui_elements[i]->Update();
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
		debug = !debug;

	return true;
}

// Called after all Updates
bool j1UI::PostUpdate()
{

	for (uint i = 0u; i < ui_elements.Count(); i++) {
		if (ui_elements[i] != nullptr) {
			ui_elements[i]->Draw(atlas);

			if (debug)
				App->render->DrawQuad({ ui_elements[i]->world_pos_x, ui_elements[i]->world_pos_y, ui_elements[i]->rect.w, ui_elements[i]->rect.h }, 255, 0, 0, 255, false);

		}
	}

	return true;
}

// Called before quitting
bool j1UI::CleanUp()
{
	LOG("Freeing GUI");

	for (uint i = 0; i < ui_elements.Count(); ++i)
	{
		if (ui_elements[i] != nullptr)
		{
			delete ui_elements[i];
			ui_elements[i] = nullptr;


			if (!ui_elements.Delete(i))
				return false;

		}
	}

	ui_elements.Clear();

	App->tex->UnLoad(atlas);
	

	return true;
}

void j1UI::DeleteUIElements() {

	LOG("Deleting all ui_elements");

	for (uint i = 0; i < ui_elements.Count(); ++i)
	{
		if (ui_elements[i] != nullptr)
		{
			delete ui_elements[i];
			ui_elements[i] = nullptr;
			ui_elements.Delete(i);
			
		}
	}

	ui_elements.Clear();

}

UIElement* j1UI::CreateLabel(int x, int y, const char* text, int size, SDL_Color color, const char* font, UIElement* parent)
{

	UIElement* aux = new UILabel(x, y, text, size, color, font, parent);


	ui_elements.PushBack(aux);


	return aux;
}

UIElement * j1UI::CreateImage(int x, int y, SDL_Rect rect, UIElement* parent)
{

	UIElement* aux = new UIImage(x, y, rect, parent);
	ui_elements.PushBack(aux);

	return aux;
}

//UIElement* j1UI::CreateImageNoAtlas(int x, int y, SDL_Rect rect, SDL_Texture* tex) {
//
//	UIElement* aux = new UIImageNoAtlas(x, y, rect, tex);
//	ui_elements.PushBack(aux);
//
//	return aux;
//}

UIElement * j1UI::CreateButton(int x, int y, SDL_Rect rect, SDL_Rect hovering, SDL_Rect clicked, UIElement* parent)
{
	UIElement* aux = new UIButton(x, y, rect, hovering, clicked, parent);
	ui_elements.PushBack(aux);

	return aux;
}


// const getter for atlas
const SDL_Texture* j1UI::GetAtlas() const
{
	return atlas;
}

// class Gui ---------------------------------------------------

