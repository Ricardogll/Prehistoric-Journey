#ifndef __BAT_H__
#define __BAT_H__

#include "Entity.h"
#include"Animation.h"



class Bat :public Entity {


public:
	Bat(int x, int y, pugi::xml_node& config, EntityTypes type);
	~Bat();

	void OnCollision(Collider* c1, Collider* c2);
	void Update(float dt);
	void Draw();
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;
	void AnimationsApplyDt();
	void LoadVariablesXML(const pugi::xml_node& entity_node);


private:

};




#endif