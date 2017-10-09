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

void Scoreboard::Update(uint32_t elapsed, EventBus *eventBus)
{
	/* handle events from game field */
	auto *events = eventBus->InGameEvents(IN_GAME_EVENT_SOURCE_GAME_FIELD);
	for (size_t i = 0, n = events->size(); i < n; i++)
	{
		switch ((*events)[i])
		{
			case INGAME_EVENT_SNAKE_DIED:
				Stop();
				Globals::gameOverLayout.SnakeDied(m_Score);
				break;

			case INGAME_EVENT_SNAKE_GROWN:
				Increment();
				break;
		}
	}

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
			Game::Instance().Events().PostInGameEvent(INGAME_EVENT_SNAKE_DIED, IN_GAME_EVENT_SOURCE_SCOREBOARD);
			Globals::gameOverLayout.SnakeDied(m_Score);
			m_TimerStopped = true;
		}
	}
}

InGameScreen::InGameScreen()
{
	m_CurrentLayout = &Globals::inGameLayout;
	m_ShouldRestart = false;

	m_Field.Initilaize(HandleInputInGame, HandleCollisionsInGame,
				RenderInGame, false, Globals::GRID_DIMENSION, Globals::GRID_DIMENSION,
				Globals::GAME_SPEED, Globals::BODY_SIZE, Globals::BORDERLESS);
}

void InGameScreen::Enter(GameEvent event)
{
	switch (event)
	{
		case GAME_EVENT_START:
			m_CurrentLayout = &Globals::inGameLayout;
			Restart();
			break;

		case GAME_EVENT_RESTART:
			m_ShouldRestart = true;
			break;
		default:
			break;
	}	
}

GameScreen* InGameScreen::HandleInput(SDL_Event *event)
{
	GameScreen *newScreen = nullptr;

	if (m_Fading)
	{
		return nullptr;
	}

	UILayout *newLayout = m_CurrentLayout->HandleInput(event, &newScreen);
	if (newLayout)
	{
		if (newScreen)
		{
			m_CurrentLayout = newLayout;
		}
		else
		{
			m_NewLayout = newLayout;

			m_Fading = true;
			m_LayoutSwitched = false;
			m_FadingTimer = 0;
		}
	}
	if (newScreen)
	{
		return newScreen;
	}

	m_Field.HandleInput(event);
	
	return nullptr;
}

void InGameScreen::Update(uint32_t elapsed, EventBus *eventBus)
{
	/* handle events */
	auto *events = eventBus->InGameEvents(IN_GAME_EVENT_SOURCE_SCOREBOARD);
	for (size_t i = 0, n = events->size(); i < n; i++)
	{
		switch ((*events)[i])
		{
			case INGAME_EVENT_SNAKE_DIED:
				m_CurrentLayout = &Globals::gameOverLayout;
				break;
		}
	}
	events = eventBus->InGameEvents(IN_GAME_EVENT_SOURCE_GAME_FIELD);
	for (size_t i = 0, n = events->size(); i < n; i++)
	{
		switch ((*events)[i])
		{
		case INGAME_EVENT_SNAKE_DIED:
			m_CurrentLayout = &Globals::gameOverLayout;
			break;
		}
	}

	m_Field.Update(elapsed, eventBus);
	m_Scoreboard.Update(elapsed, eventBus);

	if (m_Fading)
	{
		m_FadingTimer += elapsed;
		if (m_FadingTimer >= 200 && !m_LayoutSwitched)
		{
			if (m_ShouldRestart)
			{
				Restart();
				m_ShouldRestart = false;
			}

			m_CurrentLayout = m_NewLayout;
			m_NewLayout = nullptr;
			m_LayoutSwitched = true;

			m_CurrentLayout->Update(elapsed);
		}
		if (m_FadingTimer >= 400)
		{
			m_Fading = false;
		}
	}
	else
	{
		m_CurrentLayout->Update(elapsed);
	}
}

void InGameScreen::Render(SDL_Renderer *renderer)
{
	int scoreboardX = (Globals::ASPECT_RATIO == ASPECT_RATIO_4_3) ?
		30 + m_Field.GeUpLeftCornOffsetX() + 
			(m_Field.GetGridDimensionX() * m_Field.GetCellWidth()) :
		60;
	int scoreboardY = m_Field.GeUpLeftCornOffsetY();
	int scoreboardW = Globals::SCREEN_WIDTH - scoreboardX;
	int scoreboardH = Globals::SCREEN_HEIGHT - m_Field.GeUpLeftCornOffsetY();
	SDL_Rect r = { scoreboardX, scoreboardY, scoreboardW, scoreboardH };

	m_Field.Render(renderer);
	m_Scoreboard.Render(renderer, &r);
	m_CurrentLayout->Render(renderer);

	if (m_Fading)
	{
		SDL_Rect r;
		r.h = Globals::SCREEN_HEIGHT;
		r.w = Globals::SCREEN_WIDTH;
		r.x = r.y = 0;

		float f = (float)m_FadingTimer / 400;
		uint32_t alpha = 0;

		if (f <= 0.5f)
		{
			alpha = (uint32_t)((2 * f) * 255);
		}
		else
		{
			alpha = (uint32_t)((1.f - f) * 510);
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, alpha);
		SDL_RenderFillRect(renderer, &r);
	}
}

void InGameScreen::Restart()
{
	m_Field.Reconfigure(Globals::GRID_DIMENSION, Globals::GRID_DIMENSION,
		false, Globals::GAME_SPEED, Globals::BODY_SIZE, Globals::BORDERLESS, false);

	m_Scoreboard.Reset();
	m_Field.Reset();
}

void InGameScreen::Resize()
{
	m_Field.Resize(Globals::GRID_DIMENSION, Globals::GRID_DIMENSION, false, true);
}
