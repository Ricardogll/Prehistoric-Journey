#ifndef __j1UI__
#define __j1UI__

#include "j1Module.h"
#include "p2List.h"
#include "p2DynArray.h"

class UI_Element;
class SDL_Texture;

enum class UIType {

	LABEL,
	BUTTON,
	IMAGE,
	CHECKBOX,
	NONE

};


class j1UI : public j1Module{

public:

	j1UI() {}
	~j1UI();

	bool Awake(pugi::xml_node& conf);

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool CleanUp();

	bool Load(pugi::xml_node& entity_node);

	bool Save(pugi::xml_node& entity_node) const;

	bool CreateUI(int x, int y, UIType type, SDL_Texture* texture, bool clickable, p2SString string);

public:

	p2DynArray<UI_Element*> ui_elements;




};





#endif // !1
