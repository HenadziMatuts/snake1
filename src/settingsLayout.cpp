#include "settingsLayout.h"
#include "game.h"
#include "utilities.h"
#include "globals.h"

static UILayout* SettingMovementButtonEventHandler(SDL_Event *event, GameScreen **newScreen, void *userData)
{
	GameSettings *settings = (GameSettings*)userData;

	if (event->type == SDL_KEYUP)
	{
		switch (event->key.keysym.sym)
		{
			case SDLK_RETURN:
			case SDLK_SPACE:
			case SDLK_LEFT:
			case SDLK_RIGHT:
				settings->m_SmoothMovement = !(settings->m_SmoothMovement);
				break;
		}
	}

	return nullptr;
}

static UILayout* SettingColorSchemeButtonEventHandler(SDL_Event *event, GameScreen **newScreen, void *userData)
{
	GameSettings *settings = (GameSettings*)userData;
	UILayout *newLayout = nullptr;

	if (event->type == SDL_KEYUP)
	{
		switch (event->key.keysym.sym)
		{
			case SDLK_RETURN:
			case SDLK_SPACE:
			case SDLK_LEFT:
			case SDLK_RIGHT:
				settings->m_ColorScheme =
					Globals::ChangeColorScheme(event->key.keysym.sym == SDLK_LEFT ? true : false);
				Game::Instance().RebuidUI();
				break;
		}
	}

	return newLayout;
}

static UILayout* SettingResolutionButtonEventHandler(SDL_Event *event, GameScreen **newScreen, void *userData)
{
	GameSettings *settings = (GameSettings*)userData;
	UILayout *newLayout = nullptr;

	if (event->type == SDL_KEYUP)
	{
		switch (event->key.keysym.sym)
		{
		case SDLK_RETURN:
		case SDLK_SPACE:
		case SDLK_LEFT:
		case SDLK_RIGHT:
			Game::Instance().GetNextResolution(settings->m_ResolutionWidth, settings->m_ResolutionHeight,
				&settings->m_ResolutionWidth, &settings->m_ResolutionHeight,
				event->key.keysym.sym == SDLK_LEFT ? true : false);
			break;
		}
	}

	return newLayout;
}

static UILayout* SettingFullscreenButtonEventHandler(SDL_Event *event, GameScreen **newScreen, void *userData)
{
	GameSettings *settings = (GameSettings*)userData;
	UILayout *newLayout = nullptr;

	if (event->type == SDL_KEYUP)
	{
		switch (event->key.keysym.sym)
		{
			case SDLK_RETURN:
			case SDLK_SPACE:
			case SDLK_LEFT:
			case SDLK_RIGHT:
				settings->m_Fullscreen = !settings->m_Fullscreen;
				break;
			}
	}

	return newLayout;
}

static UILayout* SettingBackButtonEventHandler(SDL_Event *event, GameScreen **newScreen, void *userData)
{
	GameSettings *settings = (GameSettings*)userData;
	UILayout *newLayout = nullptr;

	if (event->type == SDL_KEYUP)
	{
		switch (event->key.keysym.sym)
		{
			case SDLK_RETURN:
			case SDLK_SPACE:
				Globals::SMOOTH_MOVEMENT = settings->m_SmoothMovement;

				Globals::menuLayout.Enter();
				newLayout = &Globals::menuLayout;
				break;
		}
	}

	return newLayout;
}

static UILayout* SettingApplyButtonEventHandler(SDL_Event *event, GameScreen **newScreen, void *userData)
{
	GameSettings *settings = (GameSettings*)userData;
	UILayout *newLayout = nullptr;

	if (event->type == SDL_KEYUP)
	{
		switch (event->key.keysym.sym)
		{
			case SDLK_RETURN:
			case SDLK_SPACE:
				if ((Globals::SCREEN_WIDTH != settings->m_ResolutionWidth)
					|| (Globals::SCREEN_HEIGHT != settings->m_ResolutionHeight))
				{
					Globals::SCREEN_WIDTH = settings->m_ResolutionWidth;
					Globals::SCREEN_HEIGHT = settings->m_ResolutionHeight;
					Game::Instance().ChangeResolution(Globals::SCREEN_WIDTH, Globals::SCREEN_HEIGHT, settings->m_Fullscreen);

					Globals::menuScreen.Resize();
					Globals::inGameScreen.Resize();
				}
				if (Globals::FULLSCREEN != settings->m_Fullscreen)
				{
					Globals::FULLSCREEN = settings->m_Fullscreen;
					Game::Instance().SetFullscreen(Globals::FULLSCREEN);

					Globals::menuScreen.Resize();
					Globals::inGameScreen.Resize();
				}

				break;
		}
	}

	return newLayout;
}

void SettingsLayout::Enter()
{
	m_Settings.m_SmoothMovement = Globals::SMOOTH_MOVEMENT;
	m_Settings.m_ColorScheme = Globals::COLOR_SCHEME->m_Name;
	m_Settings.m_ResolutionWidth = Globals::SCREEN_WIDTH;
	m_Settings.m_ResolutionHeight = Globals::SCREEN_HEIGHT;
	m_Settings.m_Fullscreen = Globals::FULLSCREEN;

	m_UIButton[m_SelectedButton].Select(false);
	m_SelectedButton = SETTINGS_UI_BUTTON_BACK;
	m_UIButton[m_SelectedButton].Select(true);
}

UILayout* SettingsLayout::HandleInput(SDL_Event *event, GameScreen **newScreen)
{
	UILayout *newLayout = nullptr;
	if (event->type == SDL_KEYUP)
	{
		switch (event->key.keysym.sym)
		{
			case SDLK_UP:
			case SDLK_DOWN:
				m_UIButton[m_SelectedButton].Select(false);

				do
				{
					m_SelectedButton = (SettingsUIButton)Utilities::ModuloSum(m_SelectedButton,
						event->key.keysym.sym == SDLK_UP ? -1 : 1, SETTINGS_UI_BUTTON_TOTAL);
				} while (!m_UIButton[m_SelectedButton].IsVisible());

				m_UIButton[m_SelectedButton].Select(true);
				break;

			case SDLK_ESCAPE:
				Globals::SMOOTH_MOVEMENT = m_Settings.m_SmoothMovement;
				newLayout = &Globals::menuLayout;
				break;

			case SDLK_LEFT:
			case SDLK_RIGHT:
				if (m_UIButton[SETTINGS_UI_BUTTON_APPLY].IsVisible())
				{
					if (m_SelectedButton == SETTINGS_UI_BUTTON_BACK && event->key.keysym.sym == SDLK_RIGHT)
					{
						m_UIButton[m_SelectedButton].Select(false);
						m_SelectedButton = SETTINGS_UI_BUTTON_APPLY;
						m_UIButton[m_SelectedButton].Select(true);
						break;
					}
					else if (m_SelectedButton == SETTINGS_UI_BUTTON_APPLY && event->key.keysym.sym == SDLK_LEFT)
					{
						m_UIButton[m_SelectedButton].Select(false);
						m_SelectedButton = SETTINGS_UI_BUTTON_BACK;
						m_UIButton[m_SelectedButton].Select(true);
						break;
					}
				}
			default:
				newLayout = m_UIButton[m_SelectedButton].HandleInput(event, newScreen, (void*)&m_Settings);
				if (m_SelectedButton == SETTINGS_UI_BUTTON_APPLY)
				{
					m_UIButton[m_SelectedButton].SetVisibility(false);
					m_UIButton[m_SelectedButton].Select(false);
					m_SelectedButton = SETTINGS_UI_BUTTON_BACK;
					m_UIButton[m_SelectedButton].Select(true);
				}

				break;
		}
	}

	return newLayout;
}

GameEvent SettingsLayout::Update(uint32_t elapsed)
{
	TTF_Font *font = Game::Instance().Resources().GetFont();
	char resolution[10];

	sprintf_s(resolution, "%dx%d", m_Settings.m_ResolutionWidth, m_Settings.m_ResolutionHeight);

	m_UILabel[SETTINGS_UI_LABEL_SMOOTH].SetVisibility(m_Settings.m_SmoothMovement);
	m_UILabel[SETTINGS_UI_LABEL_DISCRETE].SetVisibility(!m_Settings.m_SmoothMovement);
	
	m_UILabel[SETTINGS_UI_BUTTON_COLOR_SCHEME].SetText(m_Settings.m_ColorScheme, font);
	m_UILabel[SETTINGS_UI_BUTTON_RESOLUTION].SetText(resolution, font);
	m_UILabel[SETTINGS_UI_LABEL_FULLSCREEN].SetText(m_Settings.m_Fullscreen ? "yes" : "windowed", font);

	if ((m_Settings.m_Fullscreen != Globals::FULLSCREEN)
		|| (m_Settings.m_ResolutionWidth != Globals::SCREEN_WIDTH
			|| m_Settings.m_ResolutionHeight != Globals::SCREEN_HEIGHT))
	{
		m_UIButton[SETTINGS_UI_BUTTON_APPLY].SetVisibility(true);
	}
	else
	{
		m_UIButton[SETTINGS_UI_BUTTON_APPLY].SetVisibility(false);
	}

	return GAME_EVENT_NOTHING_HAPPENS;
}

void SettingsLayout::Render(SDL_Renderer *renderer)
{
	for (int i = 0; i < SETTINGS_UI_LABEL_TOTAL; i++)
	{
		m_UILabel[i].Render(renderer);
	}
	for (int i = 0; i < SETTINGS_UI_BUTTON_TOTAL; i++)
	{
		m_UIButton[i].Render(renderer);
	}
}

bool SettingsLayout::CreateLayout(SDL_Renderer *renderer)
{
	TTF_Font *font = Game::Instance().Resources().GetFont();
	SDL_Color *textc = &Globals::COLOR_SCHEME->m_Text;
	SDL_Color *selectorc = &Globals::COLOR_SCHEME->m_ButtonSelector;

	char resolution[10];
	sprintf_s(resolution, "%dx%d", m_Settings.m_ResolutionWidth, m_Settings.m_ResolutionHeight);

	if (!m_UILabel[SETTINGS_UI_LABEL_TITLE].Create("settings", font, textc, renderer, 0.5f, 0.125f, true, 0.75f)
		|| !m_UILabel[SETTINGS_UI_LABEL_SMOOTH].Create("smooth", font, textc, renderer,
			0.53125f, 0.28125f, false, 0.45f, TEXT_ANCHOR_MID_LEFT)
		|| !m_UILabel[SETTINGS_UI_LABEL_DISCRETE].Create("discrete", font, textc, renderer,
			0.53125f, 0.28125f, false, 0.45f, TEXT_ANCHOR_MID_LEFT)
		|| !m_UILabel[SETTINGS_UI_LABEL_COLOR_SCHEME].Create(m_Settings.m_ColorScheme, font, textc, renderer,
			0.53125f, 0.40625f, true, 0.45f, TEXT_ANCHOR_MID_LEFT)
		|| !m_UILabel[SETTINGS_UI_LABEL_RESOLUTION].Create(resolution, font, textc, renderer,
			0.53125f, 0.53125f, true, 0.45f, TEXT_ANCHOR_MID_LEFT)
		|| !m_UILabel[SETTINGS_UI_LABEL_FULLSCREEN].Create(m_Settings.m_Fullscreen ? "yes" : "windowed", font, textc, renderer,
			0.53125f, 0.65625f, true, 0.45f, TEXT_ANCHOR_MID_LEFT))
	{
		return false;
	}
	if (!m_UIButton[SETTINGS_UI_BUTTON_BACK].Create("back", font, textc, selectorc, renderer,
		0.46875f, 0.875f, true, SettingBackButtonEventHandler, 0.55f, TEXT_ANCHOR_MID_RIGHT)
		|| !m_UIButton[SETTINGS_UI_BUTTON_APPLY].Create("apply", font, textc, selectorc, renderer,
			0.53125f, 0.875f, false, SettingApplyButtonEventHandler, 0.55f, TEXT_ANCHOR_MID_LEFT)
		|| !m_UIButton[SETTINGS_UI_BUTTON_MOVEMENT].Create("movement", font, textc, selectorc, renderer,
			0.46875f, 0.28125f, true, SettingMovementButtonEventHandler, 0.45f, TEXT_ANCHOR_MID_RIGHT)
		|| !m_UIButton[SETTINGS_UI_BUTTON_COLOR_SCHEME].Create("color scheme", font, textc, selectorc, renderer,
			0.46875f, 0.40625f, true, SettingColorSchemeButtonEventHandler, 0.45f, TEXT_ANCHOR_MID_RIGHT)
		|| !m_UIButton[SETTINGS_UI_BUTTON_RESOLUTION].Create("screen resolution", font, textc, selectorc, renderer,
			0.46875f, 0.53125f, true, SettingResolutionButtonEventHandler, 0.45f, TEXT_ANCHOR_MID_RIGHT)
		|| !m_UIButton[SETTINGS_UI_BUTTON_FULLSCREEN].Create("fullscreen", font, textc, selectorc, renderer,
			0.46875f, 0.65625f, true, SettingFullscreenButtonEventHandler, 0.45f, TEXT_ANCHOR_MID_RIGHT))
	{
		return false;
	}
	m_UIButton[m_SelectedButton].Select(true);

	return true;
}

void SettingsLayout::DestroyLayout()
{
}