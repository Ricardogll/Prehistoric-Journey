#ifndef __CHICKEN_H__
#define __CHICKEN_H__

#include "Entity.h"
#include"Animation.h"

class Chicken :public Entity {


public:
	Chicken(int x, int y, pugi::xml_node& config, EntityTypes type);

	~Chicken();

	void OnCollision(Collider* c1, Collider* c2);
	void Update(float dt);
	void Draw();

	void AnimationsApplyDt();

private:
	fPoint prev_pos = { 0.0f,0.0f };

};

#endif