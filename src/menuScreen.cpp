#include "menuScreen.h"
#include "utilities.h"
#include "globals.h"

MenuScreen::MenuScreen()
{
	m_CurrentLayout = &Globals::menuLayout;
	m_Demo.Initilaize(HandleEventsDemo, HandleCollisionsDemo, RenderDemo, true,
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

GameScreen* MenuScreen::HandleEvents(SDL_Event *event)
{
	GameScreen *newScreen = nullptr;

	m_Demo.HandleEvents(nullptr);

	UILayout *newLayout = m_CurrentLayout->HandleEvents(event, &newScreen);
	if (newLayout)
	{
		m_CurrentLayout = newLayout;
	}

	return newScreen;
}

GameEvent MenuScreen::Update(uint32_t elapsed)
{
	m_Demo.Update(elapsed);
	return m_CurrentLayout->Update(elapsed);
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
