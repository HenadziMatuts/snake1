#include "inGameLayout.h"
#include "game.h"
#include "utilities.h"
#include "globals.h"

UILayout* InGameLayout::HandleEvents(SDL_Event *event, GameScreen **newScreen)
{
	UILayout *newLayout = nullptr;

	if (event->type == SDL_KEYUP)
	{
		switch (event->key.keysym.sym)
		{
			case SDLK_RETURN:
			case SDLK_SPACE:
				if (!m_SnakeDied && event->key.keysym.sym == SDLK_SPACE)
				{
					Globals::menuScreen.Enter(GAME_EVENT_PAUSE);
					*newScreen = &Globals::menuScreen;
				}
				else if (m_SnakeDied)
				{
					if (m_IsTryAgainSelected)
					{
						Globals::inGameScreen.Enter(GAME_EVENT_RESTART);
					}
					else
					{
						Globals::menuScreen.Enter(GAME_EVENT_STOP);
						*newScreen = &Globals::menuScreen;
					}
					m_SnakeDied = false;
				}
				break;
			case SDLK_LEFT:
				m_IsTryAgainSelected = (m_IsTryAgainSelected) ?
					m_IsTryAgainSelected : !m_IsTryAgainSelected;
				break;
			case SDLK_RIGHT:
				m_IsTryAgainSelected = (m_IsTryAgainSelected) ?
					!m_IsTryAgainSelected : m_IsTryAgainSelected;
				break;
			default:
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

		int dialogWidth = (Globals::SCREEN_WIDTH * 5) / 8;
		int dialogHeight = Globals::SCREEN_HEIGHT / 4;

		/* dialog viewport */
		r = { Globals::SCREEN_WIDTH / 5, (Globals::SCREEN_HEIGHT * 4) / 7,
			(Globals::SCREEN_WIDTH * 5) / 8, Globals::SCREEN_HEIGHT / 4 };
		SDL_RenderSetViewport(renderer, &r);

		r = { 0, 0, dialogWidth, dialogHeight };
		SDL_SetRenderDrawColor(renderer, 0xC0, 0xC0, 0xC0, 0xFF);
		SDL_RenderFillRect(renderer, &r);

		r = { dialogWidth / 20, dialogHeight / 22, 
			(dialogWidth * 9) / 10, dialogHeight / 4 };
		SDL_RenderCopy(renderer, m_CurrentMessage, NULL, &r);

		r = { (dialogWidth * 27) / 80, (dialogHeight * 9) / 22, 
			(dialogWidth * 13) / 40, dialogHeight / 4 };
		SDL_RenderCopy(renderer, m_TryAgain, NULL, &r);

		if (m_IsTryAgainSelected)
		{
			r = { (dialogWidth / 4) - 1, (dialogHeight * 8) / 11,
				(dialogWidth / 10) + 1, dialogHeight / 4 };
		}
		else
		{
			r = { ((dialogWidth * 13) / 20) - 1, (dialogHeight * 8) / 11,
				((dialogWidth * 3) / 40) + 1, dialogHeight / 4 };
		}
		SDL_SetRenderDrawColor(renderer, 0x80, 0x80, 0x80, 0xFF);
		SDL_RenderFillRect(renderer, &r);

		r = { dialogWidth / 4, (dialogHeight * 8) / 11,
			dialogWidth / 10, dialogHeight / 4 };
		SDL_RenderCopy(renderer, m_Yes, NULL, &r);

		r = { (dialogWidth * 13) / 20, (dialogHeight * 8) / 11,
			(dialogWidth * 3) / 40, dialogHeight / 4 };
		SDL_RenderCopy(renderer, m_No, NULL, &r);

		r = { 0, 0, Globals::SCREEN_WIDTH, Globals::SCREEN_HEIGHT };
		SDL_RenderSetViewport(renderer, &r);
	}
}

bool InGameLayout::CreateLayout(SDL_Renderer *renderer)
{
	if (!(m_TryAgain = Utilities::CreateTextureFromString(renderer,
		Game::Instance().Resources().GetFont(), "try again ?"))
		|| !(m_Yes = Utilities::CreateTextureFromString(renderer,
			Game::Instance().Resources().GetFont(), "yes"))
		|| !(m_No = Utilities::CreateTextureFromString(renderer,
			Game::Instance().Resources().GetFont(), "no"))
		|| !(m_DieMessages[0] = Utilities::CreateTextureFromString(renderer,
			Game::Instance().Resources().GetFont(), "not bad. for a 6 year old girl"))
		|| !(m_DieMessages[1] = Utilities::CreateTextureFromString(renderer,
			Game::Instance().Resources().GetFont(), "please do me a favor, drop it!"))
		|| !(m_DieMessages[2] = Utilities::CreateTextureFromString(renderer,
			Game::Instance().Resources().GetFont(), " maybe it's just not for you? "))
		|| !(m_DieMessages[3] = Utilities::CreateTextureFromString(renderer,
			Game::Instance().Resources().GetFont(), "  even my cat can do better  "))
		|| !(m_DieMessages[4] = Utilities::CreateTextureFromString(renderer,
			Game::Instance().Resources().GetFont(), "  and that's all you can do?  "))
		|| !(m_DieMessages[5] = Utilities::CreateTextureFromString(renderer,
			Game::Instance().Resources().GetFont(), "you can do better, keep going"))
		|| !(m_DieMessages[6] = Utilities::CreateTextureFromString(renderer,
			Game::Instance().Resources().GetFont(), "        you died !!111       "))
		|| !(m_DieMessages[7] = Utilities::CreateTextureFromString(renderer,
			Game::Instance().Resources().GetFont(), " well, you're not so hopeless "))
		|| !(m_DieMessages[8] = Utilities::CreateTextureFromString(renderer,
			Game::Instance().Resources().GetFont(), "   still not enough for me   "))
		|| !(m_DieMessages[9] = Utilities::CreateTextureFromString(renderer,
			Game::Instance().Resources().GetFont(), "    you should try harder    "))
		|| !(m_DieMessages[10] = Utilities::CreateTextureFromString(renderer,
			Game::Instance().Resources().GetFont(), "     that's a real talk!     "))
		|| !(m_DieMessages[11] = Utilities::CreateTextureFromString(renderer,
			Game::Instance().Resources().GetFont(), "yeah, show'em who's in charge"))
		|| !(m_DieMessages[12] = Utilities::CreateTextureFromString(renderer,
			Game::Instance().Resources().GetFont(), "     you gain my respect     ")))
	{
		return false;
	}

	return true;
}

void InGameLayout::DestroyLayout()
{
	SDL_DestroyTexture(m_TryAgain);
	SDL_DestroyTexture(m_Yes);
	SDL_DestroyTexture(m_No);
	SDL_DestroyTexture(m_DieMessages[0]);
	SDL_DestroyTexture(m_DieMessages[1]);
	SDL_DestroyTexture(m_DieMessages[2]);
	SDL_DestroyTexture(m_DieMessages[3]);
	SDL_DestroyTexture(m_DieMessages[4]);
	SDL_DestroyTexture(m_DieMessages[5]);
	SDL_DestroyTexture(m_DieMessages[6]);
	SDL_DestroyTexture(m_DieMessages[7]);
	SDL_DestroyTexture(m_DieMessages[8]);
	SDL_DestroyTexture(m_DieMessages[9]);
	SDL_DestroyTexture(m_DieMessages[10]);
	SDL_DestroyTexture(m_DieMessages[11]);
	SDL_DestroyTexture(m_DieMessages[12]);
}

void InGameLayout::SnakeDied(int score)
{
	if (!m_SnakeDied)
	{
		m_SnakeDied = true;

		if (score < 50)
		{
			m_CurrentMessage = m_DieMessages[Utilities::Random(AFFRONTS, NEUTRALS - 1)];
		}
		else if (score > 50 && score < 100)
		{
			m_CurrentMessage = m_DieMessages[Utilities::Random(NEUTRALS, CONGRATZ - 1)];
		}
		else
		{
			m_CurrentMessage = m_DieMessages[Utilities::Random(CONGRATZ, DIE_MESSAGES_TOTAL - 1)];
		}
	}
}