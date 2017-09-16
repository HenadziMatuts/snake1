#include "menuLayout.h"
#include "game.h"
#include "utilities.h"
#include "globals.h"

struct MenuButtonEvenHandlersData
{
	bool m_newGame;
	bool *m_quit;
};

static UILayout* MenuResumeButtonEventHandler(SDL_Event *event, GameScreen **newScreen, void *userData)
{
	if (event->type == SDL_KEYUP)
	{
		switch (event->key.keysym.sym)
		{
			case SDLK_RETURN:
			case SDLK_SPACE:
				*newScreen = &Globals::inGameScreen;
			break;
		}
	}

	return nullptr;
}

static UILayout* MenuNewGameButtonEventHandler(SDL_Event *event, GameScreen **newScreen, void *userData)
{
	UILayout *newLayout = nullptr;
	MenuButtonEvenHandlersData *data = (MenuButtonEvenHandlersData*)userData;

	if (event->type == SDL_KEYUP)
	{
		switch (event->key.keysym.sym)
		{
			case SDLK_RETURN:
			case SDLK_SPACE:
				data->m_newGame = true;
				Globals::newGameLayout.Enter();
				newLayout = &Globals::newGameLayout;
				break;
		}
	}

	return newLayout;
}

static UILayout* MenuSettingsButtonEventHandler(SDL_Event *event, GameScreen **newScreen, void *userData)
{
	UILayout *newLayout = nullptr;

	if (event->type == SDL_KEYUP)
	{
		switch (event->key.keysym.sym)
		{
			case SDLK_RETURN:
			case SDLK_SPACE:
				Globals::settingsLayout.Enter();
				newLayout = &Globals::settingsLayout;
				break;
		}
	}

	return newLayout;
}

static UILayout* MenuExitButtonEventHandler(SDL_Event *event, GameScreen **newScreen, void *userData)
{
	MenuButtonEvenHandlersData *data = (MenuButtonEvenHandlersData*)userData;

	if (event->type == SDL_KEYUP)
	{
		switch (event->key.keysym.sym)
		{
			case SDLK_RETURN:
			case SDLK_SPACE:
				*(data->m_quit) = true;
				break;
		}
	}

	return nullptr;
}

UILayout* MenuLayout::HandleInput(SDL_Event *event, GameScreen **newScreen)
{
	UILayout *newLayout = nullptr;
	
	if (m_IsSleeping)
	{
		MenuAction action = MENU_ACTION_NONE;

		switch (action = ProcessInput(event))
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
		if (action != MENU_ACTION_NONE && !m_Quit)
		{
			WakeUp();
			ResetTimer();
		}
	}
	else
	{
		if (event->type == SDL_KEYUP)
		{
			MenuButtonEvenHandlersData userData = { false, &m_Quit };
			m_WakeTime = 0;

			switch (event->key.keysym.sym)
			{
				case SDLK_UP:
				case SDLK_DOWN:
					m_UIButton[m_SelectedButton].Select(false);

					do
					{
						m_SelectedButton = (MenuUIButton)Utilities::ModuloSum(m_SelectedButton,
							event->key.keysym.sym == SDLK_UP ? -1 : 1, MENU_UI_BUTTON_TOTAL);
					} while (!m_UIButton[m_SelectedButton].IsVisible());

					m_UIButton[m_SelectedButton].Select(true);
					break;

				default:
					newLayout = m_UIButton[m_SelectedButton].HandleInput(event, newScreen, (void*)&userData);
					if (userData.m_newGame)
					{
						SetPauseLayout(false);
					}
					break;
			}
		}
	}
	
	return newLayout;
}

GameEvent MenuLayout::Update(uint32_t elapsed)
{
	if (m_Quit)
	{
		Game::Instance().Events().PostGameEvent(GAME_EVENT_QUIT);
	}
	
	/* animating invites */
	if (m_IsSleeping)
	{
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
	}
	else
	{
		if (!m_UIButton[m_SelectedButton].IsVisible())
		{
			m_UIButton[m_SelectedButton].Select(false);

			do
			{
				m_SelectedButton = (MenuUIButton)Utilities::ModuloSum(m_SelectedButton,
					1, MENU_UI_BUTTON_TOTAL);
			} while (!m_UIButton[m_SelectedButton].IsVisible());

			m_UIButton[m_SelectedButton].Select(true);
		}

		if ((m_WakeTime += elapsed) >= 5000)
		{
			FallAsleep();
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

	for (int i = 0; i < MENU_UI_BUTTON_TOTAL; i++)
	{
		m_UIButton[i].Render(renderer);
	}
}

bool MenuLayout::CreateLayout(SDL_Renderer *renderer)
{
	TTF_Font *font = Game::Instance().Resources().GetFont();
	SDL_Color *textc = &Globals::COLOR_SCHEME->m_Text;
	SDL_Color *selectorc = &Globals::COLOR_SCHEME->m_ButtonSelector;

	if (!m_UILabel[MENU_UI_TITLE].Create("snake !", font, textc, renderer, 0.5f, 0.25f, true, 1.8f)
		|| !m_UILabel[MENU_UI_INVITE_START].Create("press space to start", font, textc, renderer, 0.5f, 0.75f, false, 0.45f)
		|| !m_UILabel[MENU_UI_INVITE_SETTINGS].Create("press s to settings", font, textc, renderer, 0.5f, 0.75f, false, 0.45f)
		|| !m_UILabel[MENU_UI_INVITE_EXIT].Create("press x to win", font, textc, renderer, 0.5f, 0.75f, false, 0.45f)
		|| !m_UILabel[MENU_UI_INVITE_RESUME].Create("press space to resume", font, textc, renderer, 0.5f, 0.75f, false, 0.45f)
		|| !m_UILabel[MENU_UI_INVITE_RESTART].Create("press r to restart", font, textc, renderer, 0.5f, 0.75f, false, 0.45f)
		|| !m_UILabel[MENU_UI_INVITE_CREDITS].Create("press c to credits", font, textc, renderer, 0.5f, 0.75f, false, 0.45f))
	{
		return false;
	}

	if (!m_UIButton[MENU_UI_BUTTON_RESUME].Create("resume", font, textc,
		selectorc, renderer, 0.5f, 0.55f, false, MenuResumeButtonEventHandler, 0.45f)
		|| !m_UIButton[MENU_UI_BUTTON_NEW_GAME].Create("new game", font, textc,
			selectorc, renderer, 0.5f, 0.65f, false, MenuNewGameButtonEventHandler, 0.45f)
		|| !m_UIButton[MENU_UI_BUTTON_SETTINGS].Create("settings", font, textc,
			selectorc, renderer, 0.5f, 0.75f, false, MenuSettingsButtonEventHandler, 0.45f)
		|| !m_UIButton[MENU_UI_BUTTON_EXIT].Create("exit", font, textc,
			selectorc, renderer, 0.5f, 0.85f, false, MenuExitButtonEventHandler, 0.45f))
	{
		return false;
	}

	m_ActiveInvites.clear();
	if (m_IsPaused)
	{
		m_ActiveInvites.push_back(MENU_UI_INVITE_RESUME);
		m_ActiveInvites.push_back(MENU_UI_INVITE_RESTART);
		m_ActiveInvites.push_back(MENU_UI_INVITE_SETTINGS);
		m_ActiveInvites.push_back(MENU_UI_INVITE_EXIT);
	}
	else
	{
		m_ActiveInvites.push_back(MENU_UI_INVITE_START);
		m_ActiveInvites.push_back(MENU_UI_INVITE_SETTINGS);
		m_ActiveInvites.push_back(MENU_UI_INVITE_EXIT);
	}
	m_VisibleActiveInviteIndex = 0;

	m_SelectedButton = MENU_UI_BUTTON_NEW_GAME;
	m_UIButton[m_SelectedButton].Select(true);

	if (m_IsSleeping)
	{
		FallAsleep();
	}
	else
	{
		WakeUp();
	}

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

		m_UIButton[MENU_UI_BUTTON_RESUME].SetVisibility(true);

		m_IsPaused = true;
	}
	else if (!set && m_IsPaused)
	{
		m_ActiveInvites.clear();
		m_ActiveInvites.push_back(MENU_UI_INVITE_START);
		m_ActiveInvites.push_back(MENU_UI_INVITE_SETTINGS);
		m_ActiveInvites.push_back(MENU_UI_INVITE_EXIT);

		m_UIButton[MENU_UI_BUTTON_RESUME].SetVisibility(false);

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

MenuAction MenuLayout::ProcessInput(SDL_Event *event)
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
				return m_IsPaused ? MENU_ACTION_START_GAME : MENU_ACTION_WAKE_UP;
			case SDLK_s:
				return MENU_ACTION_SETTINGS;
			default:
				return MENU_ACTION_WAKE_UP;
		}
	}

	return MENU_ACTION_NONE;
}

void MenuLayout::WakeUp()
{
	m_IsSleeping = false;
	m_UILabel[m_ActiveInvites[m_VisibleActiveInviteIndex]].SetVisibility(false);

	m_UIButton[MENU_UI_BUTTON_RESUME].SetVisibility(m_IsPaused ? true : false);
	m_UIButton[MENU_UI_BUTTON_NEW_GAME].SetVisibility(true);
	m_UIButton[MENU_UI_BUTTON_SETTINGS].SetVisibility(true);
	m_UIButton[MENU_UI_BUTTON_EXIT].SetVisibility(true);

	m_WakeTime = 0;
}

void MenuLayout::FallAsleep()
{
	m_IsSleeping = true;
	
	m_UIButton[MENU_UI_BUTTON_RESUME].SetVisibility(false);
	m_UIButton[MENU_UI_BUTTON_NEW_GAME].SetVisibility(false);
	m_UIButton[MENU_UI_BUTTON_SETTINGS].SetVisibility(false);
	m_UIButton[MENU_UI_BUTTON_EXIT].SetVisibility(false);
}
