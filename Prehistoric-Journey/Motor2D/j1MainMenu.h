#ifndef __MAINMENU__
#define __MAINMENU__

#include "j1Module.h"

class j1MainMenu : public j1Module
{
public:

	j1MainMenu() {}

	// Destructor
	virtual ~j1MainMenu(){}

	// Called before render is available
	bool Awake(pugi::xml_node& config) { return true; }

	// Called before the first frame
	bool Start() { return true; }

	// Called before all Updates
	bool PreUpdate() { return true; }

	// Called each loop iteration
	bool Update(float dt) { return true; }

	// Called before all Updates
	bool PostUpdate() { return true; }

	// Called before quitting
	bool CleanUp() { return true; }

public:


};



#endif // !__MAINMENU__
