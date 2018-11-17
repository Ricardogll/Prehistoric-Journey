#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "Brofiler/Brofiler.h"

j1Collision::j1Collision()
{
	for (uint i = 0; i < MAX_COLLIDERS; i++)
		colliders[i] = nullptr;

	
	matrix[COLLIDER_WALL][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_WALL][COLLIDER_WALL] = false;
	matrix[COLLIDER_WALL][COLLIDER_LEDGE] = false;
	matrix[COLLIDER_WALL][COLLIDER_LIANA] = false;
	matrix[COLLIDER_WALL][COLLIDER_DEAD] = false;
	matrix[COLLIDER_WALL][COLLIDER_LEVEL_END] = false;
	matrix[COLLIDER_WALL][COLLIDER_ENEMY] = true;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER_ATTACK] = false;

	matrix[COLLIDER_PLAYER][COLLIDER_WALL] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_LEDGE] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_LIANA] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_LEVEL_END] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_DEAD] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_ENEMY] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER_ATTACK] = false;

	matrix[COLLIDER_PLAYER_ATTACK][COLLIDER_WALL] = false;
	matrix[COLLIDER_PLAYER_ATTACK][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER_ATTACK][COLLIDER_LEDGE] = false;
	matrix[COLLIDER_PLAYER_ATTACK][COLLIDER_LIANA] = false;
	matrix[COLLIDER_PLAYER_ATTACK][COLLIDER_LEVEL_END] = false;
	matrix[COLLIDER_PLAYER_ATTACK][COLLIDER_DEAD] = false;
	matrix[COLLIDER_PLAYER_ATTACK][COLLIDER_ENEMY] = true;
	matrix[COLLIDER_PLAYER_ATTACK][COLLIDER_PLAYER_ATTACK] = false;

	matrix[COLLIDER_ENEMY][COLLIDER_WALL] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_LEDGE] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_LIANA] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_LEVEL_END] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_DEAD] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_PLAYER_ATTACK] = true;


	matrix[COLLIDER_LEDGE][COLLIDER_WALL] = false;
	matrix[COLLIDER_LEDGE][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_LEDGE][COLLIDER_LEDGE] = false;
	matrix[COLLIDER_LEDGE][COLLIDER_LIANA] = false;
	matrix[COLLIDER_LEDGE][COLLIDER_LEVEL_END] = false;
	matrix[COLLIDER_LEDGE][COLLIDER_DEAD] = false;
	matrix[COLLIDER_LEDGE][COLLIDER_ENEMY] = true;
	matrix[COLLIDER_LEDGE][COLLIDER_PLAYER_ATTACK] = false;


	matrix[COLLIDER_LIANA][COLLIDER_WALL] = false;
	matrix[COLLIDER_LIANA][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_LIANA][COLLIDER_LEDGE] = false;
	matrix[COLLIDER_LIANA][COLLIDER_LIANA] = false;
	matrix[COLLIDER_LIANA][COLLIDER_LEVEL_END] = false;
	matrix[COLLIDER_LIANA][COLLIDER_DEAD] = false;
	matrix[COLLIDER_LIANA][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_LIANA][COLLIDER_PLAYER_ATTACK] = false;

	matrix[COLLIDER_LEVEL_END][COLLIDER_WALL] = false;
	matrix[COLLIDER_LEVEL_END][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_LEVEL_END][COLLIDER_LEDGE] = false;
	matrix[COLLIDER_LEVEL_END][COLLIDER_LIANA] = false;
	matrix[COLLIDER_LEVEL_END][COLLIDER_LEVEL_END] = false;
	matrix[COLLIDER_LEVEL_END][COLLIDER_DEAD] = false;
	matrix[COLLIDER_LEVEL_END][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_LEVEL_END][COLLIDER_PLAYER_ATTACK] = false;

	matrix[COLLIDER_DEAD][COLLIDER_WALL] = false;
	matrix[COLLIDER_DEAD][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_DEAD][COLLIDER_LEDGE] = false;
	matrix[COLLIDER_DEAD][COLLIDER_LIANA] = false;
	matrix[COLLIDER_DEAD][COLLIDER_LEVEL_END] = false;
	matrix[COLLIDER_DEAD][COLLIDER_DEAD] = false;
	matrix[COLLIDER_DEAD][COLLIDER_ENEMY] = true;
	matrix[COLLIDER_DEAD][COLLIDER_PLAYER_ATTACK] = false;

}

j1Collision::~j1Collision()
{}

bool j1Collision::PreUpdate()
{
	BROFILER_CATEGORY("PreUpdate Colliders", Profiler::Color::DeepPink)
	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->to_delete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

bool j1Collision::Update(float dt)
{
	BROFILER_CATEGORY("Update Colliders", Profiler::Color::MediumSpringGreen)
	Collider *c1, *c2;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty colliders
		if (colliders[i] == nullptr)
			continue;

		c1 = colliders[i];

		// avoid checking collisions already checked
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
		{
			// skip empty colliders
			if (colliders[k] == nullptr)
				continue;

			c2 = colliders[k];

			if (c1->CheckCollision(c2->rect) == true)
			{
				if (matrix[c1->type][c2->type] && c1->callback)
					c1->callback->OnCollision(c1, c2);

				if (matrix[c2->type][c1->type] && c2->callback)
					c2->callback->OnCollision(c2, c1);
			}
		}
	}



	DebugDraw();

	return true;
}

void j1Collision::DebugDraw()
{
	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		debug = !debug;

	if (debug == false)
		return;

	Uint8 alpha = 80;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		switch (colliders[i]->type)
		{
		case COLLIDER_PLAYER:
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha);
			break;
		case COLLIDER_DEAD:
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 0, alpha);
			break;
		case COLLIDER_WALL:
			App->render->DrawQuad(colliders[i]->rect, 55, 55, 55, alpha);
			break;
		case COLLIDER_LEDGE:
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 255, alpha);
			break;
		case COLLIDER_LIANA:
			App->render->DrawQuad(colliders[i]->rect, 255, 248, 220, alpha);
			break;
		case COLLIDER_LEVEL_END:
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 255, alpha);
			break;
		case COLLIDER_ENEMY:
			App->render->DrawQuad(colliders[i]->rect, 255, 153, 102, alpha);
			break;
		case COLLIDER_PLAYER_ATTACK:
			App->render->DrawQuad(colliders[i]->rect, 255, 170, 255, alpha);
			break;
			
		}

	}
}

bool j1Collision::CleanUp()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}
	return true;
}

bool j1Collision::CleanUpMap()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->type != COLLIDER_PLAYER)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}
	return true;
}

Collider* j1Collision::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, callback);
			break;
		}
	}
	return ret;
}

bool j1Collision::EraseCollider(Collider* collider)
{
	if (collider != nullptr)
	{
		// we still search for it in case we received a dangling pointer
		for (uint i = 0; i < MAX_COLLIDERS; ++i)
		{
			if (colliders[i] == collider)
			{
				collider->to_delete = true;
				break;
			}
		}
	}
	return false;
}

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	return (rect.x < r.x + r.w &&
		rect.x + rect.w > r.x &&
		rect.y < r.y + r.h &&
		rect.h + rect.y > r.y);
}