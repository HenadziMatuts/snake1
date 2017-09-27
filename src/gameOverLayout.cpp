#include "gameOverLayout.h"
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
	UILayout *newLayout = nullptr;

	if (event->type == SDL_KEYUP)
	{
		switch (event->key.keysym.sym)
		{
			case SDLK_RETURN:
			case SDLK_SPACE:
				Globals::inGameScreen.Enter(GAME_EVENT_RESTART);
				newLayout = &Globals::inGameLayout;

				break;
		}
	}

	return newLayout;
}

static UILayout* InGameNoButtonEventHandler(SDL_Event *event, GameScreen **newScreen, void *userData)
{
	UILayout *newLayout = nullptr;

	if (event->type == SDL_KEYUP)
	{
		switch (event->key.keysym.sym)
		{
			case SDLK_RETURN:
			case SDLK_SPACE:
				Globals::menuScreen.Enter(GAME_EVENT_STOP);
				*newScreen = &Globals::menuScreen;
				
				break;
		}
	}

	return newLayout;
}

UILayout* GameOverLayout::HandleInput(SDL_Event *event, GameScreen **newScreen)
{
	UILayout *newLayout = nullptr;

	if (event->type == SDL_KEYUP)
	{
		switch (event->key.keysym.sym)
		{	
			case SDLK_LEFT:
			case SDLK_RIGHT:
				m_UIButton[m_SelectedButton].Select(false);

				m_SelectedButton = (InGameUIButton)Utilities::ModuloSum(m_SelectedButton,
					event->key.keysym.sym == SDLK_LEFT ? -1 : 1, IN_GAME_UI_BUTTON_TOTAL);

				m_UIButton[m_SelectedButton].Select(true);
				break;

			default:
				newLayout = m_UIButton[m_SelectedButton].HandleInput(event, newScreen, nullptr);
				break;
		}
	}

	return newLayout;
}

GameEvent GameOverLayout::Update(uint32_t elapsed)
{
	return GAME_EVENT_NOTHING_HAPPENS;
}

void GameOverLayout::Render(SDL_Renderer *renderer)
{
	SDL_Rect r = { 0, 0, Globals::SCREEN_WIDTH, Globals::SCREEN_HEIGHT };
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xA0);
	SDL_RenderFillRect(renderer, &r);

	if (Globals::ASPECT_RATIO == ASPECT_RATIO_16_9)
	{
		m_DialogFrame.w = Globals::SCREEN_WIDTH / 2;
		m_DialogFrame.x = Globals::SCREEN_WIDTH / 4;
	}
	else
	{
		m_DialogFrame.w = (Globals::SCREEN_WIDTH * 2) / 3;
		m_DialogFrame.x = Globals::SCREEN_WIDTH / 6;
	}

	SDL_Color dialogc = Globals::COLOR_SCHEME->m_DialogBox;
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

bool GameOverLayout::CreateLayout(SDL_Renderer *renderer)
{
	TTF_Font *font = Game::Instance().Resources().GetFont();
	SDL_Color *textc = &Globals::COLOR_SCHEME->m_Text;
	SDL_Color *selectorc = &Globals::COLOR_SCHEME->m_ButtonSelector;

	if (!m_UILabel[IN_GAME_UI_LABEL_DIE_MESSAGE].Create(dieMessage[0], font, textc, renderer, 0.5f, 0.6875f, true, 0.42f)
		|| !m_UILabel[IN_GAME_UI_LABEL_TRY_AGAIN].Create("try again?", font, textc, renderer, 0.5f, 0.7875f, true, 0.42f))
	{
		return false;
	}
	if (!m_UIButton[IN_GAME_UI_BUTTON_YES].Create("yes", font, textc, selectorc, renderer,
		0.45f, 0.8875f, true, InGameYesButtonEventHandler, 0.47f, TEXT_ANCHOR_MID_RIGHT)
		|| !m_UIButton[IN_GAME_UI_BUTTON_NO].Create("no", font, textc, selectorc, renderer,
			0.56f, 0.8875f, true, InGameNoButtonEventHandler, 0.47f, TEXT_ANCHOR_MID_LEFT))
	{
		return false;
	}
	m_UIButton[m_SelectedButton].Select(true);

	m_DialogFrame.y = (Globals::SCREEN_HEIGHT * 5) / 8;
	m_DialogFrame.h = (Globals::SCREEN_HEIGHT * 5) / 16;

	SDL_Rect dim;
	m_UILabel[IN_GAME_UI_LABEL_DIE_MESSAGE].GetDimensions(&dim);

	return true;
}

void GameOverLayout::DestroyLayout()
{
}

void GameOverLayout::SnakeDied(int score)
{
	TTF_Font *font = Game::Instance().Resources().GetFont();

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