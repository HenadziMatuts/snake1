#include "inGameLayout.h"
#include "game.h"
#include "utilities.h"
#include "globals.h"

static const uint32_t DIE_MESSAGES_TOTAL = 12;
static const uint32_t AFFRONTS = 0;
static const uint32_t NEUTRALS = 5;
static const uint32_t CONGRATZ = 9;

static char *dieMessage[DIE_MESSAGES_TOTAL] = {
	"not bad. for a 6 year old girl",
	"please do me a favor, drop it!",
	"maybe it's just not for you?",
	"even my cat can do better",
	"and that's all you can do?",
	"you can do better, keep going",
	"well, you're not so hopeless",
	"still not enough for me",
	"you should try harder",
	"that's a real talk!",
	"yeah, show'em who's in charge",
	"you gain my respect"
};

static UILayout* InGameYesButtonEventHandler(SDL_Event *event, GameScreen **newScreen, void *userData)
{
	bool *snakeDied = (bool*)userData;
	UILayout *newLayout = nullptr;

	if (event->type == SDL_KEYUP)
	{
		switch (event->key.keysym.sym)
		{
			case SDLK_RETURN:
			case SDLK_SPACE:
				Globals::inGameScreen.Enter(GAME_EVENT_RESTART);

				*snakeDied = false;
				break;
		}
	}

	return newLayout;
}

static UILayout* InGameNoButtonEventHandler(SDL_Event *event, GameScreen **newScreen, void *userData)
{
	bool *snakeDied = (bool*)userData;
	UILayout *newLayout = nullptr;

	if (event->type == SDL_KEYUP)
	{
		switch (event->key.keysym.sym)
		{
		case SDLK_RETURN:
		case SDLK_SPACE:
			Globals::menuScreen.Enter(GAME_EVENT_STOP);
			*newScreen = &Globals::menuScreen;
			
			*snakeDied = false;
			break;
		}
	}

	return newLayout;
}

UILayout* InGameLayout::HandleEvents(SDL_Event *event, GameScreen **newScreen)
{
	UILayout *newLayout = nullptr;

	if (event->type == SDL_KEYUP)
	{
		switch (event->key.keysym.sym)
		{	
			case SDLK_LEFT:
			case SDLK_RIGHT:
				if (m_SnakeDied)
				{
					m_UIButton[m_SelectedButton].Select(false);

					m_SelectedButton = (InGameUIButton)Utilities::ModuloSum(m_SelectedButton,
						event->key.keysym.sym == SDLK_LEFT ? -1 : 1, IN_GAME_UI_BUTTON_TOTAL);

					m_UIButton[m_SelectedButton].Select(true);
				}
				break;

			case SDLK_RETURN:
			case SDLK_SPACE:
				if (!m_SnakeDied)
				{
					Globals::menuScreen.Enter(GAME_EVENT_PAUSE);
					*newScreen = &Globals::menuScreen;
					break;
				}
			default:
				if (m_SnakeDied)
				{
					newLayout = m_UIButton[m_SelectedButton].HandleEvents(event, newScreen, (void*)&m_SnakeDied);
				}
				break;
		}
	}

	return newLayout;
}

GameEvent InGameLayout::Update(uint32_t elapsed)
{
	return GAME_EVENT_NOTHING_HAPPENS;
}

void InGameLayout::Render(SDL_Renderer *renderer)
{
	if (m_SnakeDied)
	{
		SDL_Rect r = { 0, 0, Globals::SCREEN_WIDTH, Globals::SCREEN_HEIGHT };
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xA0);
		SDL_RenderFillRect(renderer, &r);

		if (!m_UILabel[IN_GAME_UI_LABEL_DIE_MESSAGE].GetDimensions(&r))
		{
			m_UILabel[IN_GAME_UI_LABEL_DIE_MESSAGE].Render(renderer);
			m_UILabel[IN_GAME_UI_LABEL_DIE_MESSAGE].GetDimensions(&r);

			m_DialogFrame.w = r.w + (r.w / 8);
			m_DialogFrame.x = r.x - (r.w / 16);
		}

		SDL_Color dialogc = Globals::COLOR_SCHEME.m_DialogBox;
		SDL_SetRenderDrawColor(renderer, dialogc.r, dialogc.g, dialogc.b, dialogc.a);
		SDL_RenderFillRect(renderer, &m_DialogFrame);

		for (int i = 0; i < IN_GAME_UI_LABEL_TOTAL; i++)
		{
			m_UILabel[i].Render(renderer);
		}
		for (int i = 0; i < IN_GAME_UI_BUTTON_TOTAL; i++)
		{
			m_UIButton[i].Render(renderer);
		}
	}
}

bool InGameLayout::CreateLayout(SDL_Renderer *renderer)
{
	TTF_Font *font = Game::Instance().Resources().GetFont();
	SDL_Color *textc = &Globals::COLOR_SCHEME.m_Text;
	SDL_Color *selectorc = &Globals::COLOR_SCHEME.m_ButtonSelector;

	if (!m_UILabel[IN_GAME_UI_LABEL_DIE_MESSAGE].Create(dieMessage[0], font, textc, renderer, 0.5f, 0.6875f, true, 0.42f)
		|| !m_UILabel[IN_GAME_UI_LABEL_TRY_AGAIN].Create("try again?", font, textc, renderer, 0.5f, 0.7875f, true, 0.42f))
	{
		return false;
	}
	if (!m_UIButton[IN_GAME_UI_BUTTON_YES].Create("yes", font, textc, selectorc, renderer,
		0.45f, 0.8875f, true, InGameYesButtonEventHandler, 0.5f, TEXT_ANCHOR_MID_RIGHT)
		|| !m_UIButton[IN_GAME_UI_BUTTON_NO].Create("no", font, textc, selectorc, renderer,
			0.56f, 0.8875f, true, InGameNoButtonEventHandler, 0.5f, TEXT_ANCHOR_MID_LEFT))
	{
		return false;
	}
	m_UIButton[m_SelectedButton].Select(true);

	m_DialogFrame.y = (Globals::SCREEN_HEIGHT * 5) / 8;
	m_DialogFrame.h = (Globals::SCREEN_HEIGHT * 5) / 16;

	SDL_Rect dim;
	m_UILabel[IN_GAME_UI_LABEL_DIE_MESSAGE].GetDimensions(&dim);

	m_DialogFrame.w = dim.w + (dim.w / 8);
	m_DialogFrame.x = dim.x - (dim.w / 16);
	
	return true;
}

void InGameLayout::DestroyLayout()
{
}

void InGameLayout::SnakeDied(int score)
{
	if (!m_SnakeDied)
	{
		TTF_Font *font = Game::Instance().Resources().GetFont();
		m_SnakeDied = true;

		if (score < 50)
		{
			m_UILabel[IN_GAME_UI_LABEL_DIE_MESSAGE].SetText(dieMessage[Utilities::Random(AFFRONTS, NEUTRALS - 1)], font);
		}
		else if (score > 50 && score < 100)
		{
			m_UILabel[IN_GAME_UI_LABEL_DIE_MESSAGE].SetText(dieMessage[Utilities::Random(NEUTRALS, CONGRATZ - 1)], font);
		}
		else
		{
			m_UILabel[IN_GAME_UI_LABEL_DIE_MESSAGE].SetText(dieMessage[Utilities::Random(CONGRATZ, DIE_MESSAGES_TOTAL - 1)], font);
		}
	}
}