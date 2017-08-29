#pragma once
#include "gameScreen.h"
#include "uiLayout.h"
#include "gameField.h"
#include "snake.h"
#include <cstdio>
#include <cstdint>

class Scoreboard
{
public:
	Scoreboard() :
		m_GameMode(GAME_MODE_TRAINING),
		m_Score(0),
		m_TimeLast(0),
		m_TimerStopped(true),
		m_TimerHandicap(5000)
	{
		sprintf_s(m_ScoreText, "score: %04d", m_Score);
		sprintf_s(m_DiemerText, "diemer: %03d", m_TimeLast / 1000);
	};

	void Render(SDL_Renderer *renderer, SDL_Rect *viewport);
	InGameEvent Update(uint32_t elapsed);

	void Reset();
	void Increment();
	uint32_t Score();
	void Stop();
	
private:
	GameMode m_GameMode;

	/* scoring */
	uint32_t m_Score;
	char m_ScoreText[12];

	/* timing for survival mode */
	bool m_TimerStopped;
	int m_TimeLast;
	char m_DiemerText[12];
	int m_TimerHandicap;
};

/**
 * In game screen class.
 */
class InGameScreen : public GameScreen {
public:
	InGameScreen();

	void Enter(GameEvent event);
	GameScreen* HandleEvents(SDL_Event *event);
	GameEvent Update(uint32_t elapsed);
	void Render(SDL_Renderer *renderer);

private:
	/* UI */
	UILayout *m_CurrentLayout;

	/* game field instance */
	GameField m_Field;
	/* scoreboard */
	Scoreboard m_Scoreboard;
	
	void Restart();
};