#include "Chicken.h"
#include "j1App.h"
#include "j1Collision.h"
#include "j1Entities.h"
#include "p2Log.h"
#include "p2DynArray.h"
#include "Player.h"
#include "j1Map.h"
#include "j1Audio.h"
#include "Brofiler/Brofiler.h"
#include <time.h>

Chicken::Chicken(int x, int y, pugi::xml_node& config, EntityTypes type) :Entity(x, y, type)
{
}

Chicken::~Chicken()
{
}

void Chicken::OnCollision(Collider * c1, Collider * c2)
{
}

void Chicken::Update(float dt)
{
}

void Chicken::Draw()
{
}

void Chicken::AnimationsApplyDt()
{
}
