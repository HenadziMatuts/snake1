#pragma once
#include "menuScreen.h"
#include "menuLayout.h"
#include "settingsLayout.h"
#include "inGameScreen.h"
#include "inGameLayout.h"
#include "newGameLayout.h"
#include <cstdint>
#include <cstdio>

namespace Globals
{
	/* window properties */
	extern int SCREEN_WIDTH;
	extern int SCREEN_HEIGHT;

	/* game settings */
	extern uint32_t GRID_DIMENSION;
	extern int GAME_SPEED;
	extern GameMode MODE;
	extern bool BORDERLESS;
	
	/*settings*/
	extern bool SMOOTH_MOVEMENT;

	/* log file handle */
	extern FILE *LOG;

	/* current time as ascii string */
	extern char ASCII_TIME[128];

	extern MenuScreen menuScreen;
	extern InGameScreen inGameScreen;

	extern MenuLayout menuLayout;
	extern InGameLayout inGameLayout;
	extern NewGameLayout newGameLayout;
	extern SettingsLayout settingsLayout;

	char* Time();
}