#include "game.h"
#include "utilities.h"
#include "globals.h"

void Scoreboard::Reset()
{
	m_GameMode = Globals::MODE;
	m_Score = 0;
	sprintf_s(m_ScoreText, "score: %03d", m_Score);
	
	m_TimeLast = 0;
	sprintf_s(m_DiemerText, "diemer: %03d", m_TimeLast / 1000);
	m_TimerStopped = true;
	m_TimerHandicap = (Globals::GRID_DIMENSION * 2 * 1000) / Globals::GAME_SPEED;
	m_TimerHandicap = (m_TimerHandicap * 2) / 3;
}

void Scoreboard::Increment()
{
	if (m_GameMode == GAME_MODE_SURVIVAL)
	{
		if (m_Score == 0)
		{
			m_TimerStopped = false;
		}
		m_TimeLast += m_TimerHandicap;
	}

	++m_Score;
	sprintf_s(m_ScoreText, "score: %03d", m_Score);
}

unsigned int Scoreboard::Score()
{
	return m_Score;
}

void Scoreboard::Stop()
{
	m_TimerStopped = true;
}

void Scoreboard::Render(SDL_Renderer *renderer, SDL_Rect *viewport)
{	
	TTF_Font *font = Game::Instance().Resources().GetFont();
	SDL_Color *textc = &Globals::COLOR_SCHEME->m_Text;

	if (m_ScoreLabel.Create(m_ScoreText, font, textc, renderer,
		(float)(viewport->x) / Globals::SCREEN_WIDTH,
		(float)(viewport->y) / Globals::SCREEN_HEIGHT,
		true, 0.4f, TEXT_ANCHOR_TOP_LEFT))
	{
		m_ScoreLabel.Render(renderer);
	}

	if (m_GameMode == GAME_MODE_SURVIVAL)
	{
		if (m_DiemerLabel.Create(m_DiemerText, font, textc, renderer,
			(float)(viewport->x) / Globals::SCREEN_WIDTH,
			(float)(viewport->y * 4) / Globals::SCREEN_HEIGHT,
			true, 0.4f, TEXT_ANCHOR_TOP_LEFT))
		{
			m_DiemerLabel.Render(renderer);
		}
	}
}

InGameEvent Scoreboard::Update(uint32_t elapsed)
{
	if (m_GameMode == GAME_MODE_SURVIVAL && m_Score && !m_TimerStopped)
	{
		m_TimeLast -= elapsed;
		if (m_TimeLast / 1000 > 999)
		{
			sprintf_s(m_DiemerText, "diemer: 999");
		}
		else
		{
			sprintf_s(m_DiemerText, "diemer: %03d", m_TimeLast >= 0 ? m_TimeLast / 1000 : 0);
		}
		if (m_TimeLast < 0)
		{
			return INGAME_EVENT_SNAKE_DIED;
		}
	}
	return INGAME_EVENT_NOTHING_HAPPENS;
}

InGameScreen::InGameScreen()
{
	m_CurrentLayout = &Globals::inGameLayout;
	m_Field.Initilaize(HandleEventsInGame, HandleCollisionsInGame,
				RenderInGame, false, Globals::GRID_DIMENSION,
				Globals::GAME_SPEED, Globals::BODY_SIZE, Globals::BORDERLESS);
}

void InGameScreen::Enter(GameEvent event)
{
	switch (event)
	{
		case GAME_EVENT_RESTART:
			Restart();
			break;
		default:
			break;
	}	
}

GameScreen* InGameScreen::HandleEvents(SDL_Event *event)
{
	GameScreen *newScreen = nullptr;

	UILayout *newLayout = m_CurrentLayout->HandleEvents(event, &newScreen);
	if (newLayout)
	{
		m_CurrentLayout = newLayout;
	}
	if (newScreen)
	{
		return newScreen;
	}
	m_Field.HandleEvents(event);
	
	return nullptr;
}

GameEvent InGameScreen::Update(uint32_t elapsed)
{
	switch (m_Field.Update(elapsed))
	{
		case INGAME_EVENT_SNAKE_DIED:
			m_Scoreboard.Stop();
			Globals::inGameLayout.SnakeDied(m_Scoreboard.Score());
			break;
		case INGAME_EVENT_SNAKE_GROWN:
			m_Scoreboard.Increment();
			break;
		default:
			break;
	}

	switch (m_Scoreboard.Update(elapsed))
	{
		case INGAME_EVENT_SNAKE_DIED:
			m_Field.Stop();
			Globals::inGameLayout.SnakeDied(m_Scoreboard.Score());
			break;
		default:
			break;
	}

	return m_CurrentLayout->Update(elapsed);
}

void InGameScreen::Render(SDL_Renderer *renderer)
{
	int scoreboardX = (m_Field.GeUpLeftCornOffset() * 2) + (m_Field.GetGridDimension() * m_Field.GetCellWidth());
	int scoreboardY = m_Field.GeUpLeftCornOffset();
	int scoreboardW = Globals::SCREEN_WIDTH - m_Field.GeUpLeftCornOffset() - scoreboardX;
	int scoreboardH = Globals::SCREEN_HEIGHT - m_Field.GeUpLeftCornOffset();
	SDL_Rect r = { scoreboardX, scoreboardY, scoreboardW, scoreboardH };

	m_Field.Render(renderer);
	m_Scoreboard.Render(renderer, &r);
	m_CurrentLayout->Render(renderer);
}

void InGameScreen::Restart()
{
	m_Field.Reconfigure(Globals::GRID_DIMENSION, false,
		Globals::GAME_SPEED, Globals::BODY_SIZE, Globals::BORDERLESS);
	m_Scoreboard.Reset();
	m_Field.Reset();
}