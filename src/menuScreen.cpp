#include "menuScreen.h"
#include "utilities.h"
#include "globals.h"
#include <queue>

MenuScreen::MenuScreen()
{
	m_CurrentLayout = &Globals::menuLayout;
	m_Demo.Initilaize(HandleInputDemo, HandleCollisionsDemo, RenderDemo, true,
		(uint32_t)(50 * (Globals::ASPECT_RATIO == ASPECT_RATIO_4_3 ? 1.33f : 1.78f)), 50,
		30, 10, true);
}

void MenuScreen::Enter(GameEvent event)
{
	switch (event)
	{
		case GAME_EVENT_PAUSE:
			Globals::menuLayout.SetPauseLayout(true);
			break;
		case GAME_EVENT_STOP:
			Globals::menuLayout.SetPauseLayout(false);
			break;
		default:
			break;
	}
}

GameScreen* MenuScreen::HandleInput(SDL_Event *event)
{
	GameScreen *newScreen = nullptr;

	m_Demo.HandleInput(event);

	UILayout *newLayout = m_CurrentLayout->HandleInput(event, &newScreen);
	if (newLayout)
	{
		m_CurrentLayout = newLayout;
	}

	return newScreen;
}

void MenuScreen::Update(uint32_t elapsed, EventBus *eventBus)
{
	m_Demo.Update(elapsed, eventBus);
	m_CurrentLayout->Update(elapsed);
}

void MenuScreen::Render(SDL_Renderer *renderer)
{
	m_Demo.Render(renderer);
	m_CurrentLayout->Render(renderer);
}

void MenuScreen::Resize()
{
	m_Demo.Resize((uint32_t)(50 *
		(Globals::ASPECT_RATIO == ASPECT_RATIO_4_3 ? 1.33f : 1.78f)),
		50, true, false);
	m_Demo.Reset();
}
