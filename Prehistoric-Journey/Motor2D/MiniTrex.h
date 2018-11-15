#ifndef __MINITREX_H__
#define __MINITREX_H__

#include "Entity.h"
#include"Animation.h"



class MiniTrex :public Entity {


public:
	MiniTrex(int x, int y, pugi::xml_node& config, EntityTypes type);
	~MiniTrex();

	void OnCollision(Collider* c1, Collider* c2);
	void Update(float dt);
	void Draw();
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;
	void AnimationsApplyDt();


private:

};




#endif