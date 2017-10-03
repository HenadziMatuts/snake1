#include "profileLayout.h"
#include "game.h"
#include "utilities.h"
#include "globals.h"

struct ProfileEntryButtonData
{
	bool *m_isTextInputActive;
	char *m_profileData;
	char *m_input;
	UIButton *m_this;
};

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

static UILayout* ProfileEntryButtonEventHandler(SDL_Event *event, GameScreen **newScreen, void *userData)
{
	UILayout *newLayout = nullptr;
	ProfileEntryButtonData *data = (ProfileEntryButtonData*)userData;
	TTF_Font *font = Game::Instance().Resources().GetFont();
	char temp[22] = "\0";

	if (*data->m_isTextInputActive)
	{
		if (event->type == SDL_KEYUP)
		{
			switch (event->key.keysym.sym)
			{
				case SDLK_ESCAPE:
					memset(data->m_input, 0, 21);
				case SDLK_RETURN:
					data->m_input[20] = '\0';

					strcpy_s(data->m_profileData, 21, data->m_input);
					data->m_this->SetText(*data->m_profileData ? data->m_profileData : "new profile", font);

					*data->m_isTextInputActive = false;
					SDL_StopTextInput();
					break;

				case SDLK_BACKSPACE:
					if (strlen(data->m_input))
					{
						data->m_input[strlen(data->m_input) - 1] = '\0';

						sprintf_s(temp, "%s_", data->m_input);
						data->m_this->SetText(temp, font);
					}
					break;
			}
		}
		else
		{
			if (strlen(data->m_input) < 20)
			{
				char c = SDL_tolower(event->text.text[0]);
				sprintf_s(data->m_input, 21, "%s%c", data->m_input, c);

				sprintf_s(temp, "%s_", data->m_input);
				data->m_this->SetText(temp, font);
			}	
		}	
	}
	else
	{
		if (event->type == SDL_KEYUP)
		{
			switch (event->key.keysym.sym)
			{
				case SDLK_RETURN:
				case SDLK_SPACE:
					if (*data->m_profileData == '\0')
					{
						SDL_StartTextInput();
						*data->m_isTextInputActive = true;

						memset(data->m_input, 0, 21);

						data->m_this->SetText("_", font);
					}
					break;
				}
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

	if (event->type == SDL_KEYUP || event->type == SDL_TEXTINPUT)
	{
		ProfileEntryButtonData userData;

		switch (event->key.keysym.sym)
		{
			case SDLK_UP:
			case SDLK_DOWN:
				if (!m_IsTextInputActive)
				{
					m_UIButton[m_SelectedButton].Select(false);

					do
					{
						m_SelectedButton = (ProfileUIButton)Utilities::ModuloSum(m_SelectedButton,
							event->key.keysym.sym == SDLK_UP ? -1 : 1, PROFILE_UI_BUTTON_TOTAL);
					} while (!m_UIButton[m_SelectedButton].IsVisible());

					m_UIButton[m_SelectedButton].Select(true);
				}
				break;

			case SDLK_ESCAPE:
				if (!m_IsTextInputActive)
				{
					newLayout = &Globals::menuLayout;
					break;
				}
			default:
				userData.m_isTextInputActive = &m_IsTextInputActive;
				userData.m_profileData = Globals::USER_PROFILE[m_SelectedButton - PROFILE_UI_BUTTON_FIRST_PROFILE];
				userData.m_input = m_Input;
				userData.m_this = &m_UIButton[m_SelectedButton];

				newLayout = m_UIButton[m_SelectedButton].HandleInput(event, newScreen, &userData);
				break;
		}
	}

	return newLayout;
}

GameEvent ProfileLayout::Update(uint32_t elapsed)
{
	for (int i = PROFILE_UI_BUTTON_FIRST_PROFILE, j = 0; i <= PROFILE_UI_BUTTON_LAST_PROFILE; i++, j++)
	{
		if ((j > 0) && (*Globals::USER_PROFILE[j] == '\0')
			&& (*Globals::USER_PROFILE[j - 1] == '\0'))
		{
			m_UIButton[i].SetVisibility(false);
			m_UIButton[i].Select(false);
		}
		else
		{
			m_UIButton[i].SetVisibility(true);
		}
	}

	if (!m_UIButton[m_SelectedButton].IsVisible())
	{
		do
		{
			m_SelectedButton =
				(ProfileUIButton)Utilities::ModuloSum(m_SelectedButton,
					1, PROFILE_UI_BUTTON_TOTAL);
		} while (!m_UIButton[m_SelectedButton].IsVisible());


		m_UIButton[m_SelectedButton].Select(true);
	}

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

	for (int i = PROFILE_UI_BUTTON_FIRST_PROFILE, j = 0; i <= PROFILE_UI_BUTTON_LAST_PROFILE; i++, j++)
	{
		char profile[21] = "\0";
		strcpy_s(profile, (*Globals::USER_PROFILE[j] == '\0') ?
			"new profile" : Globals::USER_PROFILE[j]);

		bool visible = true;
		if ((j > 0) && (*Globals::USER_PROFILE[j] == '\0')
			&& (*Globals::USER_PROFILE[j - 1] == '\0'))
		{
			visible = false;
		}

		if (!m_UIButton[i].Create(profile, font, textc, selectorc, renderer,
				0.5f, 0.28f + (0.11f * j), visible, ProfileEntryButtonEventHandler, 0.45f))
		{
			return false;
		}
	}

	m_UIButton[m_SelectedButton].Select(true);
	m_IsTextInputActive = false;

	return true;
}

void ProfileLayout::DestroyLayout()
{
}
