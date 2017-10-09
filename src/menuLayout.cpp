#include "menuLayout.h"
#include "game.h"
#include "utilities.h"
#include "globals.h"

struct MenuButtonEvenHandlersData
{
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

static UILayout* MenuProfileButtonEventHandler(SDL_Event *event, GameScreen **newScreen, void *userData)
{
	UILayout *newLayout = nullptr;

	if (event->type == SDL_KEYUP)
	{
		switch (event->key.keysym.sym)
		{
			case SDLK_RETURN:
			case SDLK_SPACE:
				Globals::profileLayout.Enter();
				newLayout = &Globals::profileLayout;
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

void MenuLayout::Enter()
{
}

UILayout* MenuLayout::HandleInput(SDL_Event *event, GameScreen **newScreen)
{
	UILayout *newLayout = nullptr;
	
	if (event->type == SDL_KEYUP)
	{
		MenuButtonEvenHandlersData userData = { &m_Quit };
		m_Timer = 0;

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
				break;
		}
	}
	
	return newLayout;
}

GameEvent MenuLayout::Update(uint32_t elapsed)
{
	TTF_Font *font = Game::Instance().Resources().GetFont();
	ProfileManager *profiles = &Game::Instance().Profiles();

	if (m_Quit)
	{
		Game::Instance().Events().PostGameEvent(GAME_EVENT_QUIT);
	}
	
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

	char *profileButtonLabel = profiles->GetCurrentProfileName();
	m_UIButton[MENU_UI_BUTTON_PROFILE].SetText(profileButtonLabel ? profileButtonLabel : "create profile", font);

	if ((m_Timer += elapsed) >= 5000)
	{
		Globals::sleepingMenuLayout.SetPauseLayout(m_IsPaused);
		Globals::sleepingMenuLayout.Enter();
		Globals::menuScreen.ChangeLayout(&Globals::sleepingMenuLayout);
		m_Timer = 0;
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
	bool is4_3 = Globals::ASPECT_RATIO == ASPECT_RATIO_4_3;
	
	float x = is4_3 ? 0.07f : 0.13f;
	float scale = is4_3 ? 1.6f : 1.8f;

	if (!m_UILabel[SLEEPING_MENU_UI_TITLE].Create("snake !", font, textc,
			renderer, x, 0.4f, true, scale, TEXT_ANCHOR_MID_LEFT))
	{
		return false;
	}

	x = is4_3 ? 0.82f : 0.75f;
	
	if (!m_UIButton[MENU_UI_BUTTON_RESUME].Create("resume", font, textc, selectorc, renderer,
		x, 0.17f, false, MenuResumeButtonEventHandler, 0.42f)
		|| !m_UIButton[MENU_UI_BUTTON_NEW_GAME].Create("new game", font, textc, selectorc, renderer,
			x, 0.28f, true, MenuNewGameButtonEventHandler, 0.42f)
		|| !m_UIButton[MENU_UI_BUTTON_PROFILE].Create("create profile", font, textc, selectorc, renderer,
			x, 0.39f, true, MenuProfileButtonEventHandler, 0.42f)
		|| !m_UIButton[MENU_UI_BUTTON_HIGH_SCORES].Create("high scores", font, textc, selectorc, renderer,
			x, 0.50f, true, nullptr, 0.42f)
		|| !m_UIButton[MENU_UI_BUTTON_SETTINGS].Create("settings", font, textc, selectorc, renderer,
			x, 0.61f, true, MenuSettingsButtonEventHandler, 0.42f)
		|| !m_UIButton[MENU_UI_BUTTON_CREDITS].Create("credits", font, textc, selectorc, renderer,
			x, 0.72f, true, nullptr, 0.42f)
		|| !m_UIButton[MENU_UI_BUTTON_EXIT].Create("exit", font, textc, selectorc, renderer,
			x, 0.83f, true, MenuExitButtonEventHandler, 0.42f))
	{
		return false;
	}

	m_SelectedButton = MENU_UI_BUTTON_NEW_GAME;
	m_UIButton[m_SelectedButton].Select(true);

	return true;
}

void MenuLayout::DestroyLayout()
{
}

void MenuLayout::SetPauseLayout(bool set)
{
	if (set && !m_IsPaused)
	{
		m_UIButton[MENU_UI_BUTTON_RESUME].SetVisibility(true);
		m_IsPaused = true;
	}
	else if (!set && m_IsPaused)
	{
		m_UIButton[MENU_UI_BUTTON_RESUME].SetVisibility(false);
		m_IsPaused = false;
	}
}