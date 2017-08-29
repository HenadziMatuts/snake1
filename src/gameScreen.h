#pragma once
#include <SDL.h>

enum GameMode
{
	GAME_MODE_TRAINING = 0,
	GAME_MODE_SURVIVAL,
	GAME_MODE_TOTAL
};

/**
 * Game control flow events.
 */
enum GameEvent
{
	GAME_EVENT_NOTHING_HAPPENS = 0,
	GAME_EVENT_QUIT,
	GAME_EVENT_RESUME,
	GAME_EVENT_PAUSE,
	GAME_EVENT_RESTART,
	GAME_EVENT_STOP
};

/**
 * A Game Screen interface.
 */
class GameScreen {
public:
	virtual ~GameScreen() {};

	virtual void Enter(GameEvent event) = 0;
	virtual GameScreen* HandleEvents(SDL_Event *event) = 0;
	virtual GameEvent Update(uint32_t elapsed) = 0;
	virtual void Render(SDL_Renderer *renderer) = 0;
};