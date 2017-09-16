#pragma once
#include "eventBus.h"
#include <SDL.h>

enum GameMode
{
	GAME_MODE_TRAINING = 0,
	GAME_MODE_SURVIVAL,
	GAME_MODE_TOTAL
};

/**
 * A Game Screen interface.
 */
class GameScreen {
public:
	virtual ~GameScreen() {};

	virtual void Enter(GameEvent event) = 0;
	virtual GameScreen* HandleInput(SDL_Event *event) = 0;
	virtual void Update(uint32_t elapsed, EventBus *eventBus) = 0;
	virtual void Render(SDL_Renderer *renderer) = 0;
};