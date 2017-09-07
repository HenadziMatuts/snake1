#include "menuLayout.h"
#include "game.h"
#include "utilities.h"
#include "globals.h"

UILayout* MenuLayout::HandleEvents(SDL_Event *event, GameScreen **newScreen)
{
	UILayout *newLayout = nullptr;
	MenuAction a = MENU_ACTION_NONE;

	switch (a = HandleInput(event))
	{
		case MENU_ACTION_START_GAME:
			SetPauseLayout(false);
			Globals::newGameLayout.Enter();
			newLayout = &Globals::newGameLayout;
			break;
		case MENU_ACTION_RESUME_GAME:
			*newScreen = &Globals::inGameScreen;
			break;
		case MENU_ACTION_SETTINGS:
			Globals::settingsLayout.Enter();
			newLayout = &Globals::settingsLayout;
			break;
		case MENU_ACTION_QUIT_GAME:
			m_Quit = true;
			break;
		default:
			break;
	}

	if (a != MENU_ACTION_NONE)
	{
		ResetTimer();
	}
	return newLayout;
}

GameEvent MenuLayout::Update(uint32_t elapsed)
{
	if (m_Quit)
	{
		return GAME_EVENT_QUIT;
	}

	/* animating invites */
	if ((m_Timer += elapsed) >= 750)
	{
		m_IsInviteVisible = !m_IsInviteVisible;
		m_Timer -= 750;

		if (m_IsInviteVisible)
		{
			m_UILabel[m_ActiveInvites[m_VisibleActiveInviteIndex]].SetVisibility(true);
		}
		else
		{
			m_UILabel[m_ActiveInvites[m_VisibleActiveInviteIndex]].SetVisibility(false);
			m_VisibleActiveInviteIndex =
				Utilities::ModuloSum(m_VisibleActiveInviteIndex, 1, m_ActiveInvites.size());
		}
	}

	return GAME_EVENT_NOTHING_HAPPENS;
}

void MenuLayout::Render(SDL_Renderer *renderer)
{
	for (int i = 0; i < MENU_UI_TOTAL; i++)
	{
		m_UILabel[i].Render(renderer);
	}
}

bool MenuLayout::CreateLayout(SDL_Renderer *renderer)
{
	TTF_Font *font = Game::Instance().Resources().GetFont();
	SDL_Color *textc = &Globals::COLOR_SCHEME.m_Text;

	if (!m_UILabel[MENU_UI_TITLE].Create("snake !", font, textc, renderer, 0.5f, 0.25f, true, 1.75f)
		|| !m_UILabel[MENU_UI_INVITE_START].Create("press space to start", font, textc, renderer, 0.5f, 0.75f, false, 0.45f)
		|| !m_UILabel[MENU_UI_INVITE_SETTINGS].Create("press s to settings", font, textc, renderer, 0.5f, 0.75f, false, 0.45f)
		|| !m_UILabel[MENU_UI_INVITE_EXIT].Create("press x to win", font, textc, renderer, 0.5f, 0.75f, false, 0.45f)
		|| !m_UILabel[MENU_UI_INVITE_RESUME].Create("press space to resume", font, textc, renderer, 0.5f, 0.75f, false, 0.45f)
		|| !m_UILabel[MENU_UI_INVITE_RESTART].Create("press r to restart", font, textc, renderer, 0.5f, 0.75f, false, 0.45f)
		|| !m_UILabel[MENU_UI_INVITE_CREDITS].Create("press c to credits", font, textc, renderer, 0.5f, 0.75f, false, 0.45f))
	{
		return false;
	}

	m_ActiveInvites.push_back(MENU_UI_INVITE_START);
	m_ActiveInvites.push_back(MENU_UI_INVITE_SETTINGS);
	m_ActiveInvites.push_back(MENU_UI_INVITE_EXIT);

	m_VisibleActiveInviteIndex = 0;

	return true;
}

void MenuLayout::DestroyLayout()
{
	m_ActiveInvites.clear();
}

void MenuLayout::SetPauseLayout(bool set)
{
	ResetTimer();

	if (set && !m_IsPaused)
	{
		m_ActiveInvites.clear();
		m_ActiveInvites.push_back(MENU_UI_INVITE_RESUME);
		m_ActiveInvites.push_back(MENU_UI_INVITE_RESTART);
		m_ActiveInvites.push_back(MENU_UI_INVITE_SETTINGS);
		m_ActiveInvites.push_back(MENU_UI_INVITE_EXIT);

		m_IsPaused = true;
	}
	else if (!set && m_IsPaused)
	{
		m_ActiveInvites.clear();
		m_ActiveInvites.push_back(MENU_UI_INVITE_START);
		m_ActiveInvites.push_back(MENU_UI_INVITE_SETTINGS);
		m_ActiveInvites.push_back(MENU_UI_INVITE_EXIT);

		m_IsPaused = false;
	}
}

void MenuLayout::ResetTimer()
{
	m_Timer = 0;
	m_IsInviteVisible = false;

	m_UILabel[m_ActiveInvites[m_VisibleActiveInviteIndex]].SetVisibility(false);
	m_VisibleActiveInviteIndex = 0;
}

MenuAction MenuLayout::HandleInput(SDL_Event *event)
{
	if (event->type == SDL_KEYUP)
	{
		switch (event->key.keysym.sym)
		{
			case SDLK_SPACE:
				return m_IsPaused ? MENU_ACTION_RESUME_GAME : MENU_ACTION_START_GAME;
			case SDLK_x:
				return MENU_ACTION_QUIT_GAME;
			case SDLK_r:
				return m_IsPaused ? MENU_ACTION_START_GAME : MENU_ACTION_NONE;
			case SDLK_s:
				return MENU_ACTION_SETTINGS;
			default:
				return MENU_ACTION_NONE;
		}
	}

	return MENU_ACTION_NONE;
}