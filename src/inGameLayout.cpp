#include "inGameLayout.h"
#include "eventBus.h"
#include "globals.h"

UILayout *InGameLayout::HandleInput(SDL_Event *event, GameScreen **newScreen)
{
	UILayout *newLayout = nullptr;

	if (event->type == SDL_KEYUP)
	{
		switch (event->key.keysym.sym)
		{
			case SDLK_RETURN:
			case SDLK_SPACE:
			case SDLK_ESCAPE:
				Globals::menuScreen.Enter(GAME_EVENT_PAUSE);
				*newScreen = &Globals::menuScreen;
				break;

			default:
				break;
		}
	}

	return newLayout;
}

GameEvent InGameLayout::Update(uint32_t elapsed)
{
	return GAME_EVENT_NOTHING_HAPPENS;
}

void InGameLayout::Render(SDL_Renderer *renderer)
{
}

bool InGameLayout::CreateLayout(SDL_Renderer *renderer)
{
	return true;
}

void InGameLayout::DestroyLayout()
{
}
