#pragma once
#include "menuScreen.h"
#include "menuLayout.h"
#include "settingsLayout.h"
#include "inGameScreen.h"
#include "inGameLayout.h"
#include "newGameLayout.h"
#include "colorScheme.h"
#include <cstdint>
#include <cstdio>

namespace Globals
{
	/* log file handle */
	extern FILE *LOG;
	/* default "classic grey color scheme" */
	extern ColorScheme defaultColorScheme;
	/* current time as ascii string */
	extern char ASCII_TIME[128];

	/* window properties */
	extern int SCREEN_WIDTH;
	extern int SCREEN_HEIGHT;
	extern float WINDOW_SCALE_FACTOR;

	/* new game options*/
	extern GameMode MODE;
	extern uint32_t GRID_DIMENSION;
	extern int GAME_SPEED;
	extern int BODY_SIZE;
	extern bool BORDERLESS;
	
	/*settings*/
	extern bool SMOOTH_MOVEMENT;
	extern ColorScheme *COLOR_SCHEME;

	extern MenuScreen menuScreen;
	extern InGameScreen inGameScreen;

	extern MenuLayout menuLayout;
	extern InGameLayout inGameLayout;
	extern NewGameLayout newGameLayout;
	extern SettingsLayout settingsLayout;

	char* Time();

	char* ChangeColorScheme(bool prev);
	void AddColorScheme(ColorScheme *scheme);
	bool IsUniqueColorSchemeName(char *name);
	void DestroyColorSchemes();
}
