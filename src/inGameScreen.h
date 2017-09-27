#pragma once
#include "gameScreen.h"
#include "uiLayout.h"
#include "uiWidget.h"
#include "gameField.h"
#include "snake.h"
#include <cstdio>
#include <cstdint>
#include <queue>

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
		sprintf_s(m_ScoreText, "score: %03d", m_Score);
		sprintf_s(m_DiemerText, "diemer: %03d", m_TimeLast / 1000);
	};

	void Render(SDL_Renderer *renderer, SDL_Rect *viewport);
	void Update(uint32_t elapsed, EventBus *eventBus);

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

	UILabel m_ScoreLabel, m_DiemerLabel;
};

/**
 * In game screen class.
 */
class InGameScreen : public GameScreen {
public:
	InGameScreen();

	void Enter(GameEvent event);
	GameScreen* HandleInput(SDL_Event *event);
	void Update(uint32_t elapsed, EventBus *eventBus);
	void Render(SDL_Renderer *renderer);

	void Resize();

private:
	/* UI */
	UILayout *m_CurrentLayout;

	/* game field instance */
	GameField m_Field;
	/* scoreboard */
	Scoreboard m_Scoreboard;

	void Restart();

	bool m_ShouldRestart;

	/* layouts crossfading */
	bool m_Fading;
	uint32_t m_FadingTimer;

	UILayout *m_NewLayout;
	bool m_LayoutSwitched;
};