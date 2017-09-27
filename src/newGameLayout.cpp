#include "newGameLayout.h"
#include "game.h"
#include "utilities.h"

static UILayout* NewGameStartButtonEventHandler(SDL_Event *event, GameScreen **newScreen, void *userData)
{
	NewGameSettings *settings = (NewGameSettings*)userData;
	UILayout *newLayout = nullptr;

	if (event->type == SDL_KEYUP)
	{
		switch (event->key.keysym.sym)
		{
			case SDLK_RETURN:
			case SDLK_SPACE:
				Globals::GRID_DIMENSION = settings->m_GridDimension;
				Globals::GAME_SPEED = settings->m_GameSpeed;
				Globals::MODE = settings->m_Mode;
				Globals::BODY_SIZE = settings->m_BodySize;
				Globals::BORDERLESS = settings->m_IsBorderless;

				Globals::inGameScreen.Enter(GAME_EVENT_START);
				*newScreen = &Globals::inGameScreen;

				break;
		}
	}

	return newLayout;
}

static UILayout* NewGameBackButtonEventHandler(SDL_Event *event, GameScreen **newScreen, void *userData)
{
	NewGameSettings *settings = (NewGameSettings*)userData;
	UILayout *newLayout = nullptr;

	if (event->type == SDL_KEYUP)
	{
		switch (event->key.keysym.sym)
		{
			case SDLK_RETURN:
			case SDLK_SPACE:
				Globals::GRID_DIMENSION = settings->m_GridDimension;
				Globals::GAME_SPEED = settings->m_GameSpeed;
				Globals::MODE = settings->m_Mode;
				Globals::BODY_SIZE = settings->m_BodySize;
				Globals::BORDERLESS = settings->m_IsBorderless;

				Globals::menuLayout.Enter();
				newLayout = &Globals::menuLayout;
				Globals::menuLayout.SetPauseLayout(false);
				break;
		}
	}

	return newLayout;
}

static UILayout* NewGameModeButtonEventHandler(SDL_Event *event, GameScreen **newScreen, void *userData)
{
	NewGameSettings *settings = (NewGameSettings*)userData;

	if (event->type == SDL_KEYUP)
	{
		switch (event->key.keysym.sym)
		{
			case SDLK_RETURN:
			case SDLK_SPACE:
				settings->m_Mode = (GameMode)Utilities::ModuloSum(settings->m_Mode, 1, GAME_MODE_TOTAL);
				break;

			case SDLK_LEFT:
			case SDLK_RIGHT:
				settings->m_Mode = (GameMode)Utilities::ModuloSum(settings->m_Mode,
					event->key.keysym.sym == SDLK_LEFT ? -1 : 1, GAME_MODE_TOTAL);
				break;
		}
	}

	return nullptr;
}

static UILayout* NewGameFieldSizeButtonEventHandler(SDL_Event *event, GameScreen **newScreen, void *userData)
{
	NewGameSettings *settings = (NewGameSettings*)userData;

	if (event->type == SDL_KEYUP)
	{
		switch (event->key.keysym.sym)
		{
			case SDLK_LEFT:
				settings->m_GridDimension = settings->m_GridDimension > 40 ?
					--settings->m_GridDimension : 40;
				break;

			case SDLK_RIGHT:
			case SDLK_SPACE:
			case SDLK_RETURN:
				settings->m_GridDimension = settings->m_GridDimension < 100 ?
					++settings->m_GridDimension : 100;
				break;
		}
	}

	return nullptr;
}

static UILayout* NewGameGameSpeedButtonEventHandler(SDL_Event *event, GameScreen **newScreen, void *userData)
{
	NewGameSettings *settings = (NewGameSettings*)userData;

	if (event->type == SDL_KEYUP)
	{
		switch (event->key.keysym.sym)
		{
		case SDLK_LEFT:
			settings->m_GameSpeed = settings->m_GameSpeed > 10 ?
				--settings->m_GameSpeed : 10;
			break;

		case SDLK_RIGHT:
		case SDLK_SPACE:
		case SDLK_RETURN:
			settings->m_GameSpeed = settings->m_GameSpeed < 100 ?
				++settings->m_GameSpeed : 100;
			break;
		}
	}

	return nullptr;
}

static UILayout* NewGameBodySizeButtonEventHandler(SDL_Event *event, GameScreen **newScreen, void *userData)
{
	NewGameSettings *settings = (NewGameSettings*)userData;

	if (event->type == SDL_KEYUP)
	{
		switch (event->key.keysym.sym)
		{
		case SDLK_LEFT:
			settings->m_BodySize = settings->m_BodySize > 1 ?
				--settings->m_BodySize : 1;
			break;

		case SDLK_RIGHT:
		case SDLK_SPACE:
		case SDLK_RETURN:
			settings->m_BodySize = settings->m_BodySize < 20 ?
				++settings->m_BodySize : 20;
			break;
		}
	}

	return nullptr;
}

static UILayout* NewGameBorderlessButtonEventHandler(SDL_Event *event, GameScreen **newScreen, void *userData)
{
	NewGameSettings *settings = (NewGameSettings*)userData;

	if (event->type == SDL_KEYUP)
	{
		switch (event->key.keysym.sym)
		{
		case SDLK_RETURN:
		case SDLK_SPACE:
		case SDLK_LEFT:
		case SDLK_RIGHT:
			settings->m_IsBorderless = !settings->m_IsBorderless;
			break;
		}
	}

	return nullptr;
}

void NewGameLayout::Enter()
{
	m_Settings.m_Mode = Globals::MODE;
	m_Settings.m_GridDimension = Globals::GRID_DIMENSION;
	m_Settings.m_GameSpeed = Globals::GAME_SPEED;
	m_Settings.m_BodySize = Globals::BODY_SIZE;
	m_Settings.m_IsBorderless = Globals::BORDERLESS;

	m_UIButton[m_SelectedButton].Select(false);
	m_SelectedButton = NEW_GAME_UI_BUTTON_START;
	m_UIButton[m_SelectedButton].Select(true);
	
}

UILayout *NewGameLayout::HandleInput(SDL_Event *event, GameScreen **newScreen)
{
	UILayout *newLayout = nullptr;

	if (event->type == SDL_KEYUP)
	{
		switch (event->key.keysym.sym)
		{
			case SDLK_UP:
			case SDLK_DOWN:
				m_UIButton[m_SelectedButton].Select(false);

				m_SelectedButton = (NewGameUIButton)Utilities::ModuloSum(m_SelectedButton,
					event->key.keysym.sym == SDLK_UP ? -1 : 1, NEW_GAME_UI_BUTTON_TOTAL);

				m_UIButton[m_SelectedButton].Select(true);
				break;

			case SDLK_ESCAPE:
				Globals::GRID_DIMENSION = m_Settings.m_GridDimension;
				Globals::GAME_SPEED = m_Settings.m_GameSpeed;
				Globals::MODE = m_Settings.m_Mode;
				Globals::BODY_SIZE = m_Settings.m_BodySize;
				Globals::BORDERLESS = m_Settings.m_IsBorderless;

				newLayout = &Globals::menuLayout;
				break;

			case SDLK_LEFT:
			case SDLK_RIGHT:
				if (m_SelectedButton == NEW_GAME_UI_BUTTON_START && event->key.keysym.sym == SDLK_RIGHT)
				{
					m_UIButton[m_SelectedButton].Select(false);
					m_SelectedButton = NEW_GAME_UI_BUTTON_BACK;
					m_UIButton[m_SelectedButton].Select(true);
					break;
				}
				else if (m_SelectedButton == NEW_GAME_UI_BUTTON_BACK && event->key.keysym.sym == SDLK_LEFT)
				{
					m_UIButton[m_SelectedButton].Select(false);
					m_SelectedButton = NEW_GAME_UI_BUTTON_START;
					m_UIButton[m_SelectedButton].Select(true);
					break;
				}
			default:
				newLayout = m_UIButton[m_SelectedButton].HandleInput(event, newScreen, (void*)&m_Settings);
				break;
		}
	}

	return newLayout;
}

GameEvent NewGameLayout::Update(uint32_t elapsed)
{
	char str[4];
	TTF_Font *font = Game::Instance().Resources().GetFont();

	m_UILabel[NEW_GAME_UI_LABEL_TRAINING].SetVisibility(m_Settings.m_Mode == GAME_MODE_TRAINING ? true : false);
	m_UILabel[NEW_GAME_UI_LABEL_SURVIVAL].SetVisibility(m_Settings.m_Mode == GAME_MODE_SURVIVAL ? true : false);

	m_UILabel[NEW_GAME_UI_LABEL_BORDERLESS].SetVisibility(m_Settings.m_IsBorderless);
	m_UILabel[NEW_GAME_UI_LABEL_NOT_BORDERLESS].SetVisibility(!m_Settings.m_IsBorderless);

	_itoa_s(m_Settings.m_GridDimension, str, 10);
	m_UILabel[NEW_GAME_UI_LABEL_FIELD_SIZE].SetText(str, font);

	_itoa_s(m_Settings.m_GameSpeed, str, 10);
	m_UILabel[NEW_GAME_UI_LABEL_SPEED].SetText(str, font);

	_itoa_s(m_Settings.m_BodySize, str, 10);
	m_UILabel[NEW_GAME_UI_LABEL_BODY_SIZE].SetText(str, font);

	return GAME_EVENT_NOTHING_HAPPENS;
}

void NewGameLayout::Render(SDL_Renderer *renderer)
{
	for (int i = 0; i < NEW_GAME_UI_LABEL_TOTAL; i++)
	{
		m_UILabel[i].Render(renderer);
	}
	for (int i = 0; i < NEW_GAME_UI_BUTTON_TOTAL; i++)
	{
		m_UIButton[i].Render(renderer);
	}
}

bool NewGameLayout::CreateLayout(SDL_Renderer *renderer)
{
	TTF_Font *font = Game::Instance().Resources().GetFont();
	SDL_Color *textc = &Globals::COLOR_SCHEME->m_Text;
	SDL_Color *selectorc = &Globals::COLOR_SCHEME->m_ButtonSelector;
	
	if (!m_UILabel[NEW_GAME_UI_LABEL_TITLE].Create("set up a new game", font, textc, renderer, 0.5f, 0.125f, true, 0.75f)

		|| !m_UILabel[NEW_GAME_UI_LABEL_TRAINING].Create("training", font, textc, renderer,
			0.53125f, (float)9 / 32, false, 0.45f, TEXT_ANCHOR_MID_LEFT)
		|| !m_UILabel[NEW_GAME_UI_LABEL_SURVIVAL].Create("survival", font, textc, renderer,
			0.53125f, (float)9 / 32, false, 0.45f, TEXT_ANCHOR_MID_LEFT)
		|| !m_UILabel[NEW_GAME_UI_LABEL_FIELD_SIZE].Create("40", font, textc, renderer,
			0.53125f, (float)51 / 128, true, 0.45f, TEXT_ANCHOR_MID_LEFT)
		|| !m_UILabel[NEW_GAME_UI_LABEL_SPEED].Create("10", font, textc, renderer,
			0.53125f, (float)33 / 64, true, 0.45f, TEXT_ANCHOR_MID_LEFT)
		|| !m_UILabel[NEW_GAME_UI_LABEL_BODY_SIZE].Create("10", font, textc, renderer,
			0.53125f, (float)81 / 128, true, 0.45f, TEXT_ANCHOR_MID_LEFT)
		|| !m_UILabel[NEW_GAME_UI_LABEL_BORDERLESS].Create("yes", font, textc, renderer,
			0.53125f, (float)3 / 4, false, 0.45f, TEXT_ANCHOR_MID_LEFT)
		|| !m_UILabel[NEW_GAME_UI_LABEL_NOT_BORDERLESS].Create("no", font, textc, renderer,
			0.53125f, (float)3 / 4, false, 0.45f, TEXT_ANCHOR_MID_LEFT)
		)
	{
		return false;
	}
	
	if(!m_UIButton[NEW_GAME_UI_BUTTON_MODE].Create("game mode", font, textc, selectorc, renderer,
		0.46875f, (float)9 / 32, true, NewGameModeButtonEventHandler, 0.45f, TEXT_ANCHOR_MID_RIGHT)
		|| !m_UIButton[NEW_GAME_UI_BUTTON_FIELD_SIZE].Create("field size", font, textc, selectorc, renderer,
			0.46875f, (float)51 / 128, true, NewGameFieldSizeButtonEventHandler, 0.45f, TEXT_ANCHOR_MID_RIGHT)
		|| !m_UIButton[NEW_GAME_UI_BUTTON_SPEED].Create("game speed", font, textc, selectorc, renderer,
			0.46875f, (float)33 / 64, true, NewGameGameSpeedButtonEventHandler, 0.45f, TEXT_ANCHOR_MID_RIGHT)
		|| !m_UIButton[NEW_GAME_UI_BUTTON_BODY_SIZE].Create("body size", font, textc, selectorc, renderer,
			0.46875f, (float)81 / 128, true, NewGameBodySizeButtonEventHandler, 0.45f, TEXT_ANCHOR_MID_RIGHT)
		|| !m_UIButton[NEW_GAME_UI_BUTTON_IS_BORDERLESS].Create("borderless", font, textc, selectorc, renderer,
			0.46875f, (float)3 / 4, true, NewGameBorderlessButtonEventHandler, 0.45f, TEXT_ANCHOR_MID_RIGHT)
		|| !m_UIButton[NEW_GAME_UI_BUTTON_START].Create("start", font, textc, selectorc, renderer,
			0.46875f, 0.90625f, true, NewGameStartButtonEventHandler, 0.55f, TEXT_ANCHOR_MID_RIGHT)
		|| !m_UIButton[NEW_GAME_UI_BUTTON_BACK].Create("back", font, textc, selectorc, renderer,
			0.53125f, 0.90625f, true, NewGameBackButtonEventHandler, 0.55f, TEXT_ANCHOR_MID_LEFT))
	{
		return false;
	}
	m_UIButton[m_SelectedButton].Select(true);

	return true;
}

void NewGameLayout::DestroyLayout()
{
}