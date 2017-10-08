#pragma once
#include "menuScreen.h"
#include "sleepingMenuLayout.h"
#include "menuLayout.h"
#include "settingsLayout.h"
#include "profileLayout.h"
#include "inGameScreen.h"
#include "inGameLayout.h"
#include "gameOverLayout.h"
#include "newGameLayout.h"
#include "colorScheme.h"
#include <cstdint>
#include <cstdio>

enum AspectRatio
{
	ASPECT_RATIO_4_3 = 0,
	ASPECT_RATIO_16_9,
	ASPECT_RATIO_UNKNOWN
};


namespace Globals
{
	/* log file handle */
	extern FILE *LOG;
	/* current time as ascii string */
	extern char ASCII_TIME[128];

	/* window properties */
	extern int SCREEN_WIDTH;
	extern int SCREEN_HEIGHT;
	extern float WINDOW_SCALE_FACTOR;
	extern AspectRatio ASPECT_RATIO;
	extern bool FULLSCREEN;
	
	/* new game options*/
	extern GameMode MODE;
	extern uint32_t GRID_DIMENSION;
	extern int GAME_SPEED;
	extern int BODY_SIZE;
	extern bool BORDERLESS;

	/* settings */
	extern bool SMOOTH_MOVEMENT;
	extern ColorScheme *COLOR_SCHEME;

	extern MenuScreen menuScreen;
	extern InGameScreen inGameScreen;

	extern MenuLayout menuLayout;
	extern SleepingMenuLayout sleepingMenuLayout;
	extern InGameLayout inGameLayout;
	extern GameOverLayout gameOverLayout;
	extern NewGameLayout newGameLayout;
	extern SettingsLayout settingsLayout;
	extern ProfileLayout profileLayout;

	char* Time();

	char* ChangeColorScheme(bool prev);
}
