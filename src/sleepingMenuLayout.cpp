#include "sleepingMenuLayout.h"
#include "globals.h"
#include "game.h"
#include "utilities.h"

void SleepingMenuLayout::Enter()
{
	ResetTimer();
}

UILayout* SleepingMenuLayout::HandleInput(SDL_Event *event, GameScreen **newScreen)
{
	UILayout *newLayout = nullptr;
	SleepingMenuAction action = SLEEPING_MENU_ACTION_NONE;

	switch (action = ProcessInput(event))
	{
		case SLEEPING_MENU_ACTION_START_GAME:
			Globals::newGameLayout.Enter();
			newLayout = &Globals::newGameLayout;
			break;
		case SLEEPING_MENU_ACTION_RESUME_GAME:
			*newScreen = &Globals::inGameScreen;
			break;
		case SLEEPING_MENU_ACTION_SETTINGS:
			Globals::settingsLayout.Enter();
			newLayout = &Globals::settingsLayout;
			break;
		case SLEEPING_MENU_ACTION_QUIT_GAME:
			m_Quit = true;
			break;
		default:
			break;
	}
	if (action == SLEEPING_MENU_ACTION_WAKE_UP)
	{
		Globals::menuLayout.Enter();
		newLayout = &Globals::menuLayout;
	}
	
	return newLayout;
}

GameEvent SleepingMenuLayout::Update(uint32_t elapsed)
{
	if (m_Quit)
	{
		Game::Instance().Events().PostGameEvent(GAME_EVENT_QUIT);
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

void SleepingMenuLayout::Render(SDL_Renderer *renderer)
{
	for (int i = 0; i < SLEEPING_MENU_UI_TOTAL; i++)
	{
		m_UILabel[i].Render(renderer);
	}
}

bool SleepingMenuLayout::CreateLayout(SDL_Renderer *renderer)
{
	TTF_Font *font = Game::Instance().Resources().GetFont();
	SDL_Color *textc = &Globals::COLOR_SCHEME->m_Text;
	bool is4_3 = Globals::ASPECT_RATIO == ASPECT_RATIO_4_3;

	float x = is4_3 ? 0.07f : 0.15f;
	float scale = is4_3 ? 1.6f : 1.8f;

	if (!m_UILabel[SLEEPING_MENU_UI_TITLE].Create("snake !", font, textc, renderer, x, 0.4f, true, scale, TEXT_ANCHOR_MID_LEFT)
		|| !m_UILabel[SLEEPING_MENU_UI_START].Create("press space to start", font, textc, renderer, 0.5f, 0.75f, false, 0.45f)
		|| !m_UILabel[SLEEPING_MENU_UI_SETTINGS].Create("press s to settings", font, textc, renderer, 0.5f, 0.75f, false, 0.45f)
		|| !m_UILabel[SLEEPING_MENU_UI_EXIT].Create("press x to win", font, textc, renderer, 0.5f, 0.75f, false, 0.45f)
		|| !m_UILabel[SLEEPING_MENU_UI_RESUME].Create("press space to resume", font, textc, renderer, 0.5f, 0.75f, false, 0.45f)
		|| !m_UILabel[SLEEPING_MENU_UI_RESTART].Create("press r to restart", font, textc, renderer, 0.5f, 0.75f, false, 0.45f)
		|| !m_UILabel[SLEEPING_MENU_UI_AUTHOR].Create("by henadzi matuts", font, textc, renderer, 
			is4_3 ? 0.78f : 0.75f, is4_3 ? 0.31f : 0.3f, true, 0.35f))
	{
		return false;
	}

	m_ActiveInvites.clear();
	if (m_IsPaused)
	{
		m_ActiveInvites.push_back(SLEEPING_MENU_UI_RESUME);
		m_ActiveInvites.push_back(SLEEPING_MENU_UI_RESTART);
		m_ActiveInvites.push_back(SLEEPING_MENU_UI_SETTINGS);
		m_ActiveInvites.push_back(SLEEPING_MENU_UI_EXIT);
	}
	else
	{
		m_ActiveInvites.push_back(SLEEPING_MENU_UI_START);
		m_ActiveInvites.push_back(SLEEPING_MENU_UI_SETTINGS);
		m_ActiveInvites.push_back(SLEEPING_MENU_UI_EXIT);
	}
	m_VisibleActiveInviteIndex = 0;

	return true;
}

void SleepingMenuLayout::DestroyLayout()
{
	m_ActiveInvites.clear();
}

void SleepingMenuLayout::SetPauseLayout(bool set)
{
	ResetTimer();

	if (set && !m_IsPaused)
	{
		m_ActiveInvites.clear();
		m_ActiveInvites.push_back(SLEEPING_MENU_UI_RESUME);
		m_ActiveInvites.push_back(SLEEPING_MENU_UI_RESTART);
		m_ActiveInvites.push_back(SLEEPING_MENU_UI_SETTINGS);
		m_ActiveInvites.push_back(SLEEPING_MENU_UI_EXIT);

		m_IsPaused = true;
	}
	else if (!set && m_IsPaused)
	{
		m_ActiveInvites.clear();
		m_ActiveInvites.push_back(SLEEPING_MENU_UI_START);
		m_ActiveInvites.push_back(SLEEPING_MENU_UI_SETTINGS);
		m_ActiveInvites.push_back(SLEEPING_MENU_UI_EXIT);

		m_IsPaused = false;
	}
}

SleepingMenuAction SleepingMenuLayout::ProcessInput(SDL_Event *event)
{
	if (event->type == SDL_KEYUP)
	{
		switch (event->key.keysym.sym)
		{
			case SDLK_SPACE:
				return m_IsPaused ?
					SLEEPING_MENU_ACTION_RESUME_GAME : SLEEPING_MENU_ACTION_START_GAME;
			case SDLK_x:
				return SLEEPING_MENU_ACTION_QUIT_GAME;
			case SDLK_r:
				return m_IsPaused ?
					SLEEPING_MENU_ACTION_START_GAME : SLEEPING_MENU_ACTION_WAKE_UP;
			case SDLK_s:
				return SLEEPING_MENU_ACTION_SETTINGS;
			default:
				return SLEEPING_MENU_ACTION_WAKE_UP;
		}
	}

	return SLEEPING_MENU_ACTION_NONE;
}

void SleepingMenuLayout::ResetTimer()
{
	m_Timer = 0;
	m_IsInviteVisible = false;

	m_UILabel[m_ActiveInvites[m_VisibleActiveInviteIndex]].SetVisibility(false);
	m_VisibleActiveInviteIndex = 0;
}
