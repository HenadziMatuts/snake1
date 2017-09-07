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
				settings->m_ColorScheme = (ColorSchemeName)Utilities::ModuloSum(settings->m_ColorScheme,
					event->key.keysym.sym == SDLK_LEFT ? -1 : 1, COLOR_SCHEME_TOTAL);
				Globals::ChangeColorScheme(settings->m_ColorScheme);
				Game::Instance().RebuidUI();
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
				newLayout = &Globals::menuLayout;
				break;
		}
	}

	return newLayout;
}

void SettingsLayout::Enter()
{
	m_Settings.m_SmoothMovement = Globals::SMOOTH_MOVEMENT;
	m_Settings.m_ColorScheme = Globals::COLOR_SCHEME.m_Name;

	m_UIButton[m_SelectedButton].Select(false);
	m_SelectedButton = SETTINGS_UI_BUTTON_BACK;
	m_UIButton[m_SelectedButton].Select(true);
}

UILayout* SettingsLayout::HandleEvents(SDL_Event *event, GameScreen **newScreen)
{
	UILayout *newLayout = nullptr;
	if (event->type == SDL_KEYUP)
	{
		switch (event->key.keysym.sym)
		{
			case SDLK_UP:
			case SDLK_DOWN:
				m_UIButton[m_SelectedButton].Select(false);

				m_SelectedButton = (SettingsUIButton)Utilities::ModuloSum(m_SelectedButton,
					event->key.keysym.sym == SDLK_UP ? -1 : 1, SETTINGS_UI_BUTTON_TOTAL);

				m_UIButton[m_SelectedButton].Select(true);
				break;

			case SDLK_ESCAPE:
				Globals::SMOOTH_MOVEMENT = m_Settings.m_SmoothMovement;
				newLayout = &Globals::menuLayout;
				break;

			default:
				newLayout = m_UIButton[m_SelectedButton].HandleEvents(event, newScreen, (void*)&m_Settings);
				break;
		}
	}

	return newLayout;
}

GameEvent SettingsLayout::Update(uint32_t elapsed)
{
	m_UILabel[SETTINGS_UI_LABEL_SMOOTH].SetVisibility(m_Settings.m_SmoothMovement);
	m_UILabel[SETTINGS_UI_LABEL_DISCRETE].SetVisibility(!m_Settings.m_SmoothMovement);
	
	m_UILabel[SETTINGS_UI_LABEL_SCHEME_GREY].SetVisibility(m_Settings.m_ColorScheme == COLOR_SCHEME_CLASSIC_GREY ? true : false);
	m_UILabel[SETTINGS_UI_LABEL_SCHEME_MOON].SetVisibility(m_Settings.m_ColorScheme == COLOR_SCHEME_BLUE_MOON ? true : false);

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
	SDL_Color *textc = &Globals::COLOR_SCHEME.m_Text;
	SDL_Color *selectorc = &Globals::COLOR_SCHEME.m_ButtonSelector;

	if (!m_UILabel[SETTINGS_UI_LABEL_TITLE].Create("settings", font, textc, renderer, 0.5f, 0.125f, true, 0.8f)
		|| !m_UILabel[SETTINGS_UI_LABEL_SMOOTH].Create("smooth", font, textc, renderer,
			0.53125f, 0.28152f, false, 0.45f, TEXT_ANCHOR_MID_LEFT)
		|| !m_UILabel[SETTINGS_UI_LABEL_DISCRETE].Create("discrete", font, textc, renderer,
			0.53125f, 0.28152f, false, 0.45f, TEXT_ANCHOR_MID_LEFT)
		|| !m_UILabel[SETTINGS_UI_LABEL_SCHEME_GREY].Create("classic grey", font, textc, renderer,
			0.53125f, 0.40625f, false, 0.45f, TEXT_ANCHOR_MID_LEFT)
		|| !m_UILabel[SETTINGS_UI_LABEL_SCHEME_MOON].Create("blue moon", font, textc, renderer,
			0.53125f, 0.40625f, false, 0.45f, TEXT_ANCHOR_MID_LEFT)
		)
	{
		return false;
	}
	if (!m_UIButton[SETTINGS_UI_BUTTON_BACK].Create("back", font, textc, selectorc, renderer,
		0.5f, 0.875f, true, SettingBackButtonEventHandler, 0.5f)
		|| !m_UIButton[SETTINGS_UI_BUTTON_MOVEMENT].Create("movement", font, textc, selectorc, renderer,
			0.46875f, 0.28152f, true, SettingMovementButtonEventHandler, 0.45f, TEXT_ANCHOR_MID_RIGHT)
		|| !m_UIButton[SETTINGS_UI_BUTTON_COLOR_SCHEME].Create("color scheme", font, textc, selectorc, renderer,
			0.46875f, 0.40625f, true, SettingColorSchemeButtonEventHandler, 0.45f, TEXT_ANCHOR_MID_RIGHT))
	{
		return false;
	}
	m_UIButton[m_SelectedButton].Select(true);

	return true;
}

void SettingsLayout::DestroyLayout()
{
}