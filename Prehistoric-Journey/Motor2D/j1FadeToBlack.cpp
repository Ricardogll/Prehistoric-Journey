#include <math.h>
#include "j1App.h"
#include "j1FadeToBlack.h"
#include "j1Render.h"
#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"
#include "j1Input.h"
#include "j1Player.h"

bool gate = true;
j1Module* module_offBo;
j1Module* module_onBo;

j1FadeToBlack::j1FadeToBlack() {
	//screen = { 0, 0, SCREEN_WIDTH * SCREEN_SIZE, SCREEN_HEIGHT * SCREEN_SIZE };
}

j1FadeToBlack::~j1FadeToBlack() {}

bool j1FadeToBlack::Start() {
	bool ret = true;
	//SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);
	return ret;
}

bool j1FadeToBlack::Update() {
	if (current_step == fade_step::none)
		return true;

	Uint32 now = SDL_GetTicks() - start_time;
	float normalized = MIN(1.0f, (float)now / (float)total_time);

	switch (current_step) {
	case fade_step::fade_to_black: {
		if (now >= total_time) {

			total_time += total_time;
			start_time = SDL_GetTicks();
			current_step = fade_step::fade_from_black;
		}
	} break;

	case fade_step::fade_from_black: {
		normalized = 1.0f - normalized;

		if (now >= total_time)
			current_step = fade_step::none;
	} break;
	}

	//SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, (Uint8)(normalized * 255.0f));
	//SDL_RenderFillRect(App->render->renderer, &screen);

	return true;
}

bool j1FadeToBlack::FadeToBlack(j1Module* module_off, j1Module* module_on, float time) {
	bool ret = false;
	if (current_step == fade_step::none) {
		module_onBo = module_on;
		module_offBo = module_off;
		current_step = fade_step::fade_to_black;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time * 0.5f * 1000.0f);
		ret = true;
	}
	return ret;
}