#include "newGameLayout.h"
#include "game.h"
#include "utilities.h"

void NewGameLayout::Enter()
{
	m_Settings.m_GridDimension = Globals::GRID_DIMENSION;
	m_Settings.m_GameSpeed = Globals::GAME_SPEED;
	m_Settings.m_Mode = Globals::MODE;
	m_Settings.m_IsBorderless = Globals::BORDERLESS;
}

UILayout *NewGameLayout::HandleEvents(SDL_Event *event, GameScreen **newScreen)
{
	UILayout *newLayout = nullptr;

	if (event->type == SDL_KEYUP)
	{
		switch (event->key.keysym.sym)
		{
			case SDLK_UP:
			case SDLK_DOWN:
				m_Selected = (NewGameSettingsInteractable)Utilities::ModuloSum(m_Selected,
					event->key.keysym.sym == SDLK_UP ? -1 : 1, NEW_GAME_SETTINGS_TOTAL);
				break;

			case SDLK_LEFT:
			case SDLK_RIGHT:
				switch (m_Selected)
				{
					case NEW_GAME_MODE:
						m_Settings.m_Mode = (GameMode)Utilities::ModuloSum(m_Settings.m_Mode,
							event->key.keysym.sym == SDLK_LEFT ? -1 : 1, GAME_MODE_TOTAL);
						m_ShowConcreteModeTip = true;
						break;

					case NEW_GAME_FIELD_SIZE:
						if (m_Settings.m_GridDimension > 40 && event->key.keysym.sym == SDLK_LEFT)
						{
							m_Settings.m_GridDimension--;
						}
						else if (m_Settings.m_GridDimension < 100 && event->key.keysym.sym == SDLK_RIGHT)
						{
							m_Settings.m_GridDimension++;
						}
						break;

					case NEW_GAME_SPEED:
						if (m_Settings.m_GameSpeed > 10 && event->key.keysym.sym == SDLK_LEFT)
						{
							m_Settings.m_GameSpeed--;
						}
						else if (m_Settings.m_GameSpeed < 100 && event->key.keysym.sym == SDLK_RIGHT)
						{
							m_Settings.m_GameSpeed++;
						}
						break;

					case NEW_GAME_IS_BORDERLESS:
						m_Settings.m_IsBorderless = !m_Settings.m_IsBorderless;
						break;

					case NEW_GAME_START:
						m_Selected = (NewGameSettingsInteractable)Utilities::ModuloSum(m_Selected,
							event->key.keysym.sym == SDLK_RIGHT ? 1 : 0, NEW_GAME_SETTINGS_TOTAL);
						break;

					case NEW_GAME_BACK:
						m_Selected = (NewGameSettingsInteractable)Utilities::ModuloSum(m_Selected,
							event->key.keysym.sym == SDLK_LEFT ? -1 : 0, NEW_GAME_SETTINGS_TOTAL);
						break;

					default:
						break;
				}
				break;
				
			case SDLK_RETURN:
			case SDLK_SPACE:
				switch (m_Selected)
				{
					case NEW_GAME_START:
						Globals::GRID_DIMENSION = m_Settings.m_GridDimension;
						Globals::GAME_SPEED = m_Settings.m_GameSpeed;
						Globals::MODE = m_Settings.m_Mode;
						Globals::BORDERLESS = m_Settings.m_IsBorderless;

						newLayout = &Globals::menuLayout;

						Globals::inGameScreen.Enter(GAME_EVENT_RESTART);
						*newScreen = &Globals::inGameScreen;
						break;

					case NEW_GAME_BACK:
						Globals::GRID_DIMENSION = m_Settings.m_GridDimension;
						Globals::GAME_SPEED = m_Settings.m_GameSpeed;
						Globals::MODE = m_Settings.m_Mode;
						Globals::BORDERLESS = m_Settings.m_IsBorderless;

						newLayout = &Globals::menuLayout;
						break;

					case NEW_GAME_MODE:
						m_Settings.m_Mode = (GameMode)Utilities::ModuloSum(m_Settings.m_Mode, 1, GAME_MODE_TOTAL);
						m_ShowConcreteModeTip = true;
						break;

					case NEW_GAME_IS_BORDERLESS:
						m_Settings.m_IsBorderless = !m_Settings.m_IsBorderless;
						break;
				}
				break;
			
			case SDLK_ESCAPE:
				Globals::GRID_DIMENSION = m_Settings.m_GridDimension;
				Globals::GAME_SPEED = m_Settings.m_GameSpeed;
				Globals::MODE = m_Settings.m_Mode;

				newLayout = &Globals::menuLayout;
				break;

			default:
				break;
		}
	}
	return newLayout;
}

GameEvent NewGameLayout::Update(uint32_t elapsed)
{
	if (m_Selected != NEW_GAME_MODE)
	{
		m_ShowConcreteModeTip = false;
	}

	switch (m_Selected)
	{
		case NEW_GAME_MODE:
			if (m_ShowConcreteModeTip)
			{
				if (m_Settings.m_Mode == GAME_MODE_TRAINING)
				{
					m_Tip = NEW_GAME_TIPS_MODE_TRAINING;
				}
				else if (m_Settings.m_Mode == GAME_MODE_SURVIVAL)
				{
					m_Tip = NEW_GAME_TIPS_MODE_SURVIVAL;
				}
			}
			else
			{
				m_Tip = NEW_GAME_TIPS_MODE;
			}
			break;
		case NEW_GAME_FIELD_SIZE:
			m_Tip = NEW_GAME_TIPS_FIELD_SIZE;
			break;
		case NEW_GAME_SPEED:
			m_Tip = NEW_GAME_TIPS_SPEED;
			break;
		case NEW_GAME_START:
			m_Tip = NEW_GAME_TIPS_START;
			break;
		case NEW_GAME_BACK:
			m_Tip = NEW_GAME_TIPS_BACK;
			break;
	}

	return GAME_EVENT_NOTHING_HAPPENS;
}

void NewGameLayout::Render(SDL_Renderer *renderer)
{
	SDL_Rect r;

	r = { Globals::SCREEN_WIDTH / 6, Globals::SCREEN_HEIGHT / 16,
		(Globals::SCREEN_WIDTH * 2) / 3, Globals::SCREEN_HEIGHT / 8 };
	SDL_RenderCopy(renderer, m_Title, nullptr, &r);

	/* options */
	r = { (Globals::SCREEN_WIDTH * 15) / 64, Globals::SCREEN_HEIGHT / 4,
		Globals::SCREEN_WIDTH / 4, Globals::SCREEN_HEIGHT / 14 };
	for (int i = 0; i < NEW_GAME_SETTINGS_TOTAL; i++, r.y += Globals::SCREEN_HEIGHT / 9)
	{
		if (i == NEW_GAME_BACK)
		{
			r.y -= Globals::SCREEN_HEIGHT / 9;
			r.x += (Globals::SCREEN_WIDTH * 5) / 16;
		}
		if (i == NEW_GAME_START)
		{
			r.y += Globals::SCREEN_HEIGHT / 32;
		}

		if (m_Selected == i)
		{
			SDL_Rect _r = r;
			switch (m_Selected)
			{
				case NEW_GAME_MODE:
				case NEW_GAME_SPEED:
				case NEW_GAME_IS_BORDERLESS:
					_r.y -= 3;
					_r.w += 4;
					_r.x -= 3;
					_r.h += 6;
					break;
				case NEW_GAME_FIELD_SIZE:
					_r.y -= 3;
					_r.w += 4;
					_r.x -= 3;
					break;
				case NEW_GAME_BACK:
				case NEW_GAME_START:
					_r.y -= 3;
					_r.x -= 3;
					break;
				default:
					break;
			}
			SDL_SetRenderDrawColor(renderer, 0x90, 0x90, 0x90, 0x80);
			SDL_RenderFillRect(renderer, &_r);
		}

		SDL_RenderCopy(renderer, m_Interactables[i], nullptr, &r);

		/* values */
		SDL_Rect _r = r;
		SDL_Texture *numTexture = nullptr;
		char numStr[16];
		switch (i)
		{
			case NEW_GAME_FIELD_SIZE:
				sprintf_s(numStr, "%d", m_Settings.m_GridDimension);
				break;
			case NEW_GAME_SPEED:
				sprintf_s(numStr, "%d", m_Settings.m_GameSpeed);
				break;
			default:
				break;
		}
		switch (i)
		{
			case NEW_GAME_FIELD_SIZE:
			case NEW_GAME_SPEED:
				numTexture = Utilities::CreateTextureFromString(renderer,
					Game::Instance().Resources().GetFont(), numStr);
				if (numTexture)
				{
					_r.x += (Globals::SCREEN_WIDTH * 3) / 8;
					_r.w /= 2;
					SDL_RenderCopy(renderer, numTexture, nullptr, &_r);
					SDL_DestroyTexture(numTexture);
				}
				break;
			case NEW_GAME_MODE:
				_r.x += (Globals::SCREEN_WIDTH * 5) / 16;
				SDL_RenderCopy(renderer, m_GameModes[m_Settings.m_Mode], nullptr, &_r);
				break;
			case NEW_GAME_IS_BORDERLESS:
				_r.x += (Globals::SCREEN_WIDTH * 3) / 8;
				_r.w /= 2;
				SDL_RenderCopy(renderer, m_Settings.m_IsBorderless ? m_Yes : m_No, nullptr, &_r);
			default:
				break;
		}
	}

	if (m_Selected != NEW_GAME_IS_BORDERLESS)
	{
		r = { Globals::SCREEN_WIDTH / 3, (Globals::SCREEN_HEIGHT * 7) / 8,
			Globals::SCREEN_WIDTH / 3, Globals::SCREEN_HEIGHT / 24 };
		SDL_RenderCopy(renderer, m_Tips[m_Tip], nullptr, &r);
	}
}

bool NewGameLayout::CreateLayout(SDL_Renderer * renderer)
{
	if (!(m_Title = Utilities::CreateTextureFromString(renderer,
		Game::Instance().Resources().GetFont(), "set up a new game"))
		|| !(m_Interactables[NEW_GAME_MODE] = Utilities::CreateTextureFromString(renderer,
			Game::Instance().Resources().GetFont(), "game mode"))
		|| !(m_Interactables[NEW_GAME_FIELD_SIZE] = Utilities::CreateTextureFromString(renderer,
			Game::Instance().Resources().GetFont(), "field size"))
		| !(m_Interactables[NEW_GAME_SPEED] = Utilities::CreateTextureFromString(renderer,
			Game::Instance().Resources().GetFont(), "game speed"))
		|| !(m_Interactables[NEW_GAME_IS_BORDERLESS] = Utilities::CreateTextureFromString(renderer,
			Game::Instance().Resources().GetFont(), "borderless"))
		|| !(m_Interactables[NEW_GAME_BACK] = Utilities::CreateTextureFromString(renderer,
			Game::Instance().Resources().GetFont(), "   back   "))
		|| !(m_Interactables[NEW_GAME_START] = Utilities::CreateTextureFromString(renderer,
			Game::Instance().Resources().GetFont(), "  start  "))
		|| !(m_GameModes[GAME_MODE_TRAINING] = Utilities::CreateTextureFromString(renderer,
			Game::Instance().Resources().GetFont(), "training"))
		|| !(m_GameModes[GAME_MODE_SURVIVAL] = Utilities::CreateTextureFromString(renderer,
			Game::Instance().Resources().GetFont(), "survival"))
		|| !(m_Yes = Utilities::CreateTextureFromString(renderer,
			Game::Instance().Resources().GetFont(), "yes"))
		|| !(m_No = Utilities::CreateTextureFromString(renderer,
			Game::Instance().Resources().GetFont(), "no"))
		|| !(m_Tips[NEW_GAME_TIPS_MODE] = Utilities::CreateTextureFromString(renderer,
			Game::Instance().Resources().GetFont(), "   choose game mode   "))
		|| !(m_Tips[NEW_GAME_TIPS_MODE_TRAINING] = Utilities::CreateTextureFromString(renderer,
			Game::Instance().Resources().GetFont(), "       free run       "))
		|| !(m_Tips[NEW_GAME_TIPS_MODE_SURVIVAL] = Utilities::CreateTextureFromString(renderer,
			Game::Instance().Resources().GetFont(), "      eat or die      "))
		|| !(m_Tips[NEW_GAME_TIPS_FIELD_SIZE] = Utilities::CreateTextureFromString(renderer,
			Game::Instance().Resources().GetFont(), "adjust game field size"))
		|| !(m_Tips[NEW_GAME_TIPS_SPEED] = Utilities::CreateTextureFromString(renderer,
			Game::Instance().Resources().GetFont(), "  adjust game speed  "))
		|| !(m_Tips[NEW_GAME_TIPS_START] = Utilities::CreateTextureFromString(renderer,
			Game::Instance().Resources().GetFont(), "    start the game    "))
		|| !(m_Tips[NEW_GAME_TIPS_BACK] = Utilities::CreateTextureFromString(renderer,
			Game::Instance().Resources().GetFont(), "     back to menu     "))
		)
	{
		return false;
	}

	m_Selected = NEW_GAME_START;
	m_Tip = NEW_GAME_TIPS_START;

	return true;
}

void NewGameLayout::DestroyLayout()
{
	SDL_DestroyTexture(m_Title);
	for (int i = 0; i < NEW_GAME_SETTINGS_TOTAL; i++)
	{
		SDL_DestroyTexture(m_Interactables[i]);
	}
	for (int i = 0; i < GAME_MODE_TOTAL; i++)
	{
		SDL_DestroyTexture(m_GameModes[i]);
	}
	SDL_DestroyTexture(m_Yes);
	SDL_DestroyTexture(m_No);
}
