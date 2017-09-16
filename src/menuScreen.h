#pragma once
#include "gameScreen.h"
#include "uiLayout.h"
#include "gameField.h"
#include <SDL.h>

/**
 * Main menu class.
 */
class MenuScreen : public GameScreen {
public:
	MenuScreen();

	void Enter(GameEvent event);
	GameScreen* HandleInput(SDL_Event *event);
	void Update(uint32_t elapsed, EventBus *eventBus);
	void Render(SDL_Renderer *renderer);

	void Resize();

private:
	UILayout *m_CurrentLayout;

	GameField m_Demo;
};
