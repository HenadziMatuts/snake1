#include "profileLayout.h"
#include "game.h"
#include "utilities.h"
#include "globals.h"

struct ProfileEntryButtonData
{
	bool *m_isTextInputActive;
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

static UILayout* ProfileCreateButtonEventHandler(SDL_Event *event, GameScreen **newScreen, void *userData)
{
	UILayout *newLayout = nullptr;
	ProfileEntryButtonData *data = (ProfileEntryButtonData*)userData;
	TTF_Font *font = Game::Instance().Resources().GetFont();
	ProfileManager *profiles = &Game::Instance().Profiles();
	char temp[MAX_PROFILE_NAME_SIZE + 1] = "\0";

	if (*data->m_isTextInputActive)
	{
		if (event->type == SDL_KEYUP)
		{
			switch (event->key.keysym.sym)
			{
				case SDLK_ESCAPE:
					memset(data->m_input, 0, MAX_PROFILE_NAME_SIZE);
				case SDLK_RETURN:
					data->m_input[MAX_PROFILE_NAME_SIZE - 1] = '\0';
					
					if (strlen(data->m_input))
					{
						profiles->CreateProfile(data->m_input);
					}
					
					*data->m_isTextInputActive = false;
					SDL_StopTextInput();

					newLayout = &Globals::profileLayout;
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
			if (strlen(data->m_input) < MAX_PROFILE_NAME_SIZE - 1)
			{
				char c = SDL_tolower(event->text.text[0]);
				sprintf_s(data->m_input, MAX_PROFILE_NAME_SIZE, "%s%c", data->m_input, c);

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
					if (profiles->HasFreeSlot())
					{
						SDL_StartTextInput();
						*data->m_isTextInputActive = true;

						memset(data->m_input, 0, MAX_PROFILE_NAME_SIZE);
						data->m_this->SetText("_", font);
					}
					break;
			}
		}
	}

	return newLayout;
}

static UILayout* ProfileDeleteButtonEventHandler(SDL_Event *event, GameScreen **newScreen, void *userData)
{
	UILayout *newLayout = nullptr;

	if (event->type == SDL_KEYUP)
	{
		switch (event->key.keysym.sym)
		{
			case SDLK_RETURN:
			case SDLK_SPACE:
				Game::Instance().Profiles().DeleteCurrentProfile();
				newLayout = &Globals::profileLayout;
				break;
		}
	}

	return newLayout;
}

void ProfileLayout::Enter()
{
	m_UIButton[m_SelectedButton].Select(false);
	m_SelectedButton = PROFILE_UI_BUTTON_NEW;
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
	TTF_Font *font = Game::Instance().Resources().GetFont();
	ProfileManager *profiles = &Game::Instance().Profiles();
	char *profileName = nullptr;
	char title[64];

	if (profileName = profiles->GetCurrentProfileName())
	{
		sprintf_s(title, "profile : %s", profileName);
		m_UILabel[PROFILE_UI_LABEL_TITLE].SetText(title, font);

		m_UIButton[PROFILE_UI_BUTTON_CHANGE].SetVisibility(true);
		m_UIButton[PROFILE_UI_BUTTON_DELETE].SetVisibility(true);
	}
	else
	{
		m_UILabel[PROFILE_UI_LABEL_TITLE].SetText("profile management", font);
		m_UIButton[PROFILE_UI_BUTTON_CHANGE].SetVisibility(false);
		m_UIButton[PROFILE_UI_BUTTON_CHANGE].Select(false);

		m_UIButton[PROFILE_UI_BUTTON_DELETE].SetVisibility(false);
		m_UIButton[PROFILE_UI_BUTTON_DELETE].Select(false);
	}

	if (!m_IsTextInputActive)
	{
		m_UIButton[PROFILE_UI_BUTTON_NEW].SetText("create", font);
	}

	if (!m_UIButton[m_SelectedButton].IsVisible())
	{
		do
		{
			m_SelectedButton =
				(ProfileUIButton)Utilities::ModuloSum(m_SelectedButton,
					-1, PROFILE_UI_BUTTON_TOTAL);
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
	ProfileManager *profiles = &Game::Instance().Profiles();
	SDL_Color *textc = &Globals::COLOR_SCHEME->m_Text;
	SDL_Color *selectorc = &Globals::COLOR_SCHEME->m_ButtonSelector;

	if (!m_UILabel[PROFILE_UI_LABEL_TITLE].Create("profile management", 
		font, textc, renderer, 0.5f, 0.125f, true, 0.75f))
	{
		return false;
	}

	if (!m_UIButton[PROFILE_UI_BUTTON_BACK].Create("back to menu", font, textc, selectorc,
		renderer, 0.5f, 0.875f, true, ProfileBackButtonEventHandler, 0.55f)
		|| !m_UIButton[PROFILE_UI_BUTTON_CHANGE].Create("switch", font, textc, selectorc,
			renderer, 0.5f, 0.375f, false, nullptr, 0.45f)
		|| !m_UIButton[PROFILE_UI_BUTTON_NEW].Create("create", font, textc, selectorc,
			renderer, 0.5f, 0.5f, true, ProfileCreateButtonEventHandler, 0.45f)
		|| !m_UIButton[PROFILE_UI_BUTTON_DELETE].Create("delete", font, textc, selectorc,
			renderer, 0.5f, 0.625f, false, ProfileDeleteButtonEventHandler, 0.45f))
	{
		return false;
	}

	m_UIButton[m_SelectedButton].Select(true);
	m_IsTextInputActive = false;

	return true;
}

void ProfileLayout::DestroyLayout()
{
}
