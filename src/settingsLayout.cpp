#include "settingsLayout.h"
#include "game.h"
#include "utilities.h"
#include "globals.h"

void SettingsLayout::Enter()
{
	m_SmoothMovement = Globals::SMOOTH_MOVEMENT;
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
				m_Selected = (SettingsInteractable)Utilities::ModuloSum(m_Selected,
					event->key.keysym.sym == SDLK_UP ? -1 : 1, SETTINGS_TOTAL);
				break;

			case SDLK_LEFT:
			case SDLK_RIGHT:
				switch (m_Selected)
				{
					case SETTINGS_SMOOTH_MOVEMENT:
						m_SmoothMovement = !m_SmoothMovement;
						break;

					default:
						break;
				}
				break;

			case SDLK_RETURN:
			case SDLK_SPACE:
				switch (m_Selected)
				{
					case SETTINGS_SMOOTH_MOVEMENT:
						m_SmoothMovement = !m_SmoothMovement;
						break;

					case SETTINGS_BACK:
						Globals::SMOOTH_MOVEMENT = m_SmoothMovement;
						newLayout = &Globals::menuLayout;
						break;
				}
				break;

			case SDLK_ESCAPE:
				newLayout = &Globals::menuLayout;
				break;

			default:
				break;
		}
	}
	return newLayout;
}

GameEvent SettingsLayout::Update(uint32_t elapsed)
{
	return GAME_EVENT_NOTHING_HAPPENS;
}

void SettingsLayout::Render(SDL_Renderer *renderer)
{
	SDL_Rect r;

	r = { (Globals::SCREEN_WIDTH * 11) / 32, Globals::SCREEN_HEIGHT / 16,
		(Globals::SCREEN_WIDTH * 5) / 16, Globals::SCREEN_HEIGHT / 8 };
	SDL_RenderCopy(renderer, m_Title, nullptr, &r);

	/* buttons */
	r = { (Globals::SCREEN_WIDTH * 15) / 64, Globals::SCREEN_HEIGHT / 4,
		Globals::SCREEN_WIDTH / 4, Globals::SCREEN_HEIGHT / 12 };
	for (int i = 0; i < SETTINGS_TOTAL; i++, r.y += Globals::SCREEN_HEIGHT / 8)
	{
		if (i == SETTINGS_BACK)
		{
			r.y = (Globals::SCREEN_HEIGHT * 3) / 4;
		}

		if (m_Selected == i)
		{
			SDL_Rect _r = r;
			switch (m_Selected)
			{
				case SETTINGS_SMOOTH_MOVEMENT:
					_r.y -= 3;
					_r.w += 4;
					_r.x -= 3;
					_r.h += 6;
					break;
				case SETTINGS_BACK:
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

		/* numbers */
		SDL_Rect _r = r;
		switch (i)
		{
			case SETTINGS_SMOOTH_MOVEMENT:
				_r.x += (Globals::SCREEN_WIDTH * 5) / 16;
				SDL_RenderCopy(renderer, m_SmoothMovement ? m_Smooth : m_Discrete, nullptr, &_r);
				break;
			default:
				break;
		}
	}
}

bool SettingsLayout::CreateLayout(SDL_Renderer *renderer)
{
	if (!(m_Title = Utilities::CreateTextureFromString(renderer,
		Game::Instance().Resources().GetFont(), "settings"))
		|| !(m_Interactables[SETTINGS_SMOOTH_MOVEMENT] = Utilities::CreateTextureFromString(renderer,
			Game::Instance().Resources().GetFont(), "movement"))
		|| !(m_Interactables[SETTINGS_BACK] = Utilities::CreateTextureFromString(renderer,
			Game::Instance().Resources().GetFont(), "   back   "))
		|| !(m_Smooth = Utilities::CreateTextureFromString(renderer,
			Game::Instance().Resources().GetFont(), "smooth"))
		|| !(m_Discrete = Utilities::CreateTextureFromString(renderer,
			Game::Instance().Resources().GetFont(), "discrete")))
	{
		return false;
	}

	m_Selected = SETTINGS_BACK;

	return true;
}

void SettingsLayout::DestroyLayout()
{
	SDL_DestroyTexture(m_Title);
	SDL_DestroyTexture(m_Interactables[SETTINGS_SMOOTH_MOVEMENT]);
	SDL_DestroyTexture(m_Interactables[SETTINGS_BACK]);
	SDL_DestroyTexture(m_Smooth);
	SDL_DestroyTexture(m_Discrete);
}