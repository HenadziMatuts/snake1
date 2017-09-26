#include "profileLayout.h"
#include "game.h"
#include "utilities.h"
#include "globals.h"

static UILayout* ProfileBackButtonEventHandler(SDL_Event *event, GameScreen **newScreen, void *userData)
{
	UILayout *newLayout = nullptr;

	if (event->type == SDL_KEYUP)
	{
		switch (event->key.keysym.sym)
		{
			case SDLK_RETURN:
			case SDLK_SPACE:
				Globals::menuLayout.Enter();
				newLayout = &Globals::menuLayout;
				break;
			}
	}

	return newLayout;
}

void ProfileLayout::Enter()
{
	m_UIButton[m_SelectedButton].Select(false);
	m_SelectedButton = PROFILE_UI_BUTTON_BACK;
	m_UIButton[m_SelectedButton].Select(true);
}

UILayout* ProfileLayout::HandleInput(SDL_Event *event, GameScreen **newScreen)
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
					m_SelectedButton = (ProfileUIButton)Utilities::ModuloSum(m_SelectedButton,
						event->key.keysym.sym == SDLK_UP ? -1 : 1, PROFILE_UI_BUTTON_TOTAL);
				} while (!m_UIButton[m_SelectedButton].IsVisible());

				m_UIButton[m_SelectedButton].Select(true);
				break;

			case SDLK_ESCAPE:
				newLayout = &Globals::menuLayout;
				break;

			default:
				newLayout = m_UIButton[m_SelectedButton].HandleInput(event, newScreen, nullptr);
				break;
		}
	}

	return newLayout;
}

GameEvent ProfileLayout::Update(uint32_t elapsed)
{
	return GAME_EVENT_NOTHING_HAPPENS;
}

void ProfileLayout::Render(SDL_Renderer *renderer)
{
	for (int i = 0; i < PROFILE_UI_LABEL_TOTAL; i++)
	{
		m_UILabel[i].Render(renderer);
	}
	for (int i = 0; i < PROFILE_UI_BUTTON_TOTAL; i++)
	{
		m_UIButton[i].Render(renderer);
	}
}

bool ProfileLayout::CreateLayout(SDL_Renderer *renderer)
{
	TTF_Font *font = Game::Instance().Resources().GetFont();
	SDL_Color *textc = &Globals::COLOR_SCHEME->m_Text;
	SDL_Color *selectorc = &Globals::COLOR_SCHEME->m_ButtonSelector;

	if (!m_UILabel[PROFILE_UI_LABEL_TITLE].Create("profile selection", font, textc, renderer, 0.5f, 0.125f, true, 0.75f))
	{
		return false;
	}

	if (!m_UIButton[PROFILE_UI_BUTTON_BACK].Create("back", font, textc, selectorc,
		renderer, 0.5f, 0.875f, true, ProfileBackButtonEventHandler, 0.55f))
	{
		return false;
	}

	for (int i = PROFILE_UI_BUTTON_BACK + 1, j = 0; i <= PROFILE_UI_BUTTON_PROFILE; i++, j++)
	{
		if (!m_UIButton[i].Create("new profile", font, textc, selectorc,
			renderer, 0.5f, 0.3f + (0.1f * j), j == 0 ? true : false, nullptr, 0.45f))
		{
			return false;
		}
	}

	m_UIButton[m_SelectedButton].Select(true);

	return true;
}

void ProfileLayout::DestroyLayout()
{
}
