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
	GameScreen* HandleEvents(SDL_Event *event);
	GameEvent Update(uint32_t elapsed);
	void Render(SDL_Renderer *renderer);

private:
	UILayout *m_CurrentLayout;

	GameField m_Demo;
};
