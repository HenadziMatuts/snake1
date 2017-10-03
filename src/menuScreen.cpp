#include "menuScreen.h"
#include "utilities.h"
#include "globals.h"
#include <queue>

MenuScreen::MenuScreen()
{
	m_Fading = false;
	m_LayoutSwitched = true;
	m_FadingTimer = 0;

	m_CurrentLayout = &Globals::sleepingMenuLayout;
	m_Demo.Initilaize(HandleInputDemo, HandleCollisionsDemo, RenderDemo, true,
		(uint32_t)(50 * (Globals::ASPECT_RATIO == ASPECT_RATIO_4_3 ? 1.33f : 1.78f)), 50,
		30, 10, true);
}

void MenuScreen::Enter(GameEvent event)
{
	switch (event)
	{
		case GAME_EVENT_PAUSE:
			Globals::menuLayout.Enter();
			Globals::menuLayout.SetPauseLayout(true);
			m_CurrentLayout = &Globals::menuLayout;
			break;
		case GAME_EVENT_STOP:
			Globals::menuLayout.Enter();
			Globals::menuLayout.SetPauseLayout(false);
			m_CurrentLayout = &Globals::menuLayout;

			break;
		default:
			break;
	}
}

GameScreen* MenuScreen::HandleInput(SDL_Event *event)
{
	GameScreen *newScreen = nullptr;

	if (m_Fading)
	{
		return nullptr;
	}

	m_Demo.HandleInput(event);

	UILayout *newLayout = m_CurrentLayout->HandleInput(event, &newScreen);
	if (newLayout)
	{
		if (newScreen)
		{
			m_CurrentLayout = newLayout;
		}
		else
		{
			m_NewLayout = newLayout;

			m_Fading = true;
			m_LayoutSwitched = false;
			m_FadingTimer = 0;
		}
	}

	return newScreen;
}

void MenuScreen::Update(uint32_t elapsed, EventBus *eventBus)
{
	m_Demo.Update(elapsed, eventBus);
	m_CurrentLayout->Update(elapsed);
	
	if (m_Fading)
	{
		m_FadingTimer += elapsed;
		if (m_FadingTimer >= 200 && !m_LayoutSwitched)
		{
			m_CurrentLayout = m_NewLayout;
			m_NewLayout = nullptr;
			m_LayoutSwitched = true;
		}
		if (m_FadingTimer >= 400)
		{
			m_Fading = false;
		}
	}
}

void MenuScreen::Render(SDL_Renderer *renderer)
{
	m_Demo.Render(renderer);
	m_CurrentLayout->Render(renderer);

	if (m_Fading)
	{
		SDL_Rect r;
		r.h = Globals::SCREEN_HEIGHT;
		r.w = Globals::SCREEN_WIDTH;
		r.x = r.y = 0;

		float f = (float)m_FadingTimer / 400;
		uint32_t alpha = 0;

		if (f <= 0.5f)
		{
			alpha = (uint32_t)((2 * f) * 255);
		}
		else
		{
			alpha = (uint32_t)((1.f - f) * 510);
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, alpha);
		SDL_RenderFillRect(renderer, &r);
	}
}

void MenuScreen::Resize()
{
	m_Demo.Resize((uint32_t)(50 *
		(Globals::ASPECT_RATIO == ASPECT_RATIO_4_3 ? 1.33f : 1.78f)),
		50, true, false);
	m_Demo.Reset();
}

void MenuScreen::ChangeLayout(UILayout *newLayout)
{
	m_NewLayout = newLayout;

	m_Fading = true;
	m_LayoutSwitched = false;
	m_FadingTimer = 0;
}
