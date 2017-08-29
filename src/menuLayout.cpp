#include "menuLayout.h"
#include "game.h"
#include "utilities.h"
#include "globals.h"

UILayout* MenuLayout::HandleEvents(SDL_Event *event, GameScreen **newScreen)
{
	UILayout *newLayout = nullptr;

	switch (HandleInput(event))
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

	ResetTimer();
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

		if (!m_IsInviteVisible)
		{
			m_CurrentActiveIndex = Utilities::ModuloSum(m_CurrentActiveIndex, 1, m_ActiveInvites.size());
		}
	}

	return GAME_EVENT_NOTHING_HAPPENS;
}

void MenuLayout::Render(SDL_Renderer *renderer)
{
	SDL_Rect r = { Globals::SCREEN_WIDTH / 8, Globals::SCREEN_HEIGHT / 12,
		(Globals::SCREEN_WIDTH * 3) / 4, Globals::SCREEN_HEIGHT / 3 };
	SDL_RenderCopy(renderer, m_Title, nullptr, &r);

	if (m_IsInviteVisible)
	{
		r = { Globals::SCREEN_WIDTH / 5, (Globals::SCREEN_HEIGHT * 3) / 4, 
			(Globals::SCREEN_WIDTH * 3) / 5, Globals::SCREEN_HEIGHT / 16 };
		SDL_RenderCopy(renderer, m_Invite[m_ActiveInvites[m_CurrentActiveIndex]], nullptr, &r);
	}
}

bool MenuLayout::CreateLayout(SDL_Renderer *renderer)
{
	if (!(m_Title = Utilities::CreateTextureFromString(renderer,
		Game::Instance().Resources().GetFont(), "snake !"))
		|| !(m_Invite[MENU_INVITE_START] = Utilities::CreateTextureFromString(renderer,
			Game::Instance().Resources().GetFont(), "press space to start"))
		|| !(m_Invite[MENU_INVITE_SETTINGS] = Utilities::CreateTextureFromString(renderer,
			Game::Instance().Resources().GetFont(), "press s to settings"))
		|| !(m_Invite[MENU_INVITE_EXIT] = Utilities::CreateTextureFromString(renderer,
			Game::Instance().Resources().GetFont(), "   press x to win   "))
		|| !(m_Invite[MENU_INVITE_RESUME] = Utilities::CreateTextureFromString(renderer,
			Game::Instance().Resources().GetFont(), "press space to resume"))
		|| !(m_Invite[MENU_INVITE_RESTART] = Utilities::CreateTextureFromString(renderer,
			Game::Instance().Resources().GetFont(), " press r to restart "))
		|| !(m_Invite[MENU_INVITE_CREDITS] = Utilities::CreateTextureFromString(renderer,
			Game::Instance().Resources().GetFont(), " press c to credits ")))
	{
		return false;
	}

	m_ActiveInvites.push_back(MENU_INVITE_START);
	m_ActiveInvites.push_back(MENU_INVITE_SETTINGS);
	m_ActiveInvites.push_back(MENU_INVITE_EXIT);
	m_CurrentActiveIndex = 0;

	return true;
}

void MenuLayout::DestroyLayout()
{
	SDL_DestroyTexture(m_Title);
	SDL_DestroyTexture(m_Invite[0]);
	SDL_DestroyTexture(m_Invite[1]);
	SDL_DestroyTexture(m_Invite[2]);
	SDL_DestroyTexture(m_Invite[3]);
	SDL_DestroyTexture(m_Invite[4]);
	SDL_DestroyTexture(m_Invite[5]);
}

void MenuLayout::SetPauseLayout(bool set)
{
	ResetTimer();

	if (set && !m_IsPaused)
	{
		m_ActiveInvites.clear();
		m_ActiveInvites.push_back(MENU_INVITE_RESUME);
		m_ActiveInvites.push_back(MENU_INVITE_RESTART);
		m_ActiveInvites.push_back(MENU_INVITE_SETTINGS);
		m_ActiveInvites.push_back(MENU_INVITE_EXIT);
		m_CurrentActiveIndex = 0;

		m_IsPaused = true;
	}
	else if (!set && m_IsPaused)
	{
		m_ActiveInvites.clear();
		m_ActiveInvites.push_back(MENU_INVITE_START);
		m_ActiveInvites.push_back(MENU_INVITE_SETTINGS);
		m_ActiveInvites.push_back(MENU_INVITE_EXIT);
		m_CurrentActiveIndex = 0;

		m_IsPaused = false;
	}
}

void MenuLayout::ResetTimer()
{
	m_Timer = 0;
	m_CurrentActiveIndex = 0;
	m_IsInviteVisible = false;
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