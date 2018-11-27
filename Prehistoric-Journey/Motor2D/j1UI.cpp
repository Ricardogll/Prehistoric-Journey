#include "j1UI.h"

j1UI::~j1UI()
{
}

bool j1UI::Awake(pugi::xml_node & conf)
{
	return true;
}

bool j1UI::Start()
{
	return true;
}

bool j1UI::PreUpdate()
{
	return true;
}

bool j1UI::Update(float dt)
{
	return true;
}

bool j1UI::CleanUp()
{
	return true;
}

bool j1UI::Load(pugi::xml_node & entity_node)
{
	return true;
}

bool j1UI::Save(pugi::xml_node & entity_node) const
{
	return true;
}

bool j1UI::CreateUI(int x, int y, UIType type, SDL_Texture* texture, bool clickable, p2SString string)
{
	return true;
}
