#pragma once
#include "menuScreen.h"
#include "menuLayout.h"
#include "settingsLayout.h"
#include "inGameScreen.h"
#include "inGameLayout.h"
#include "newGameLayout.h"
#include <cstdint>
#include <cstdio>

enum ColorSchemeName
{
	COLOR_SCHEME_CLASSIC_GREY = 0,
	COLOR_SCHEME_SUNRISE,
	COLOR_SCHEME_BLUE_MOON,
	COLOR_SCHEME_TOTAL
};

struct ColorScheme
{
	ColorScheme() :
		m_Name(COLOR_SCHEME_CLASSIC_GREY)
	{
		m_Bg = { 0xE0, 0xE0, 0xE0, 0xFF };

		m_Text = { 0x00, 0x00, 0x00, 0xFF };
		m_ButtonSelector = { 0x90, 0x90, 0x90, 0x80 };
		m_DialogBox = { 0xC0, 0xC0, 0xC0, 0xFF };

		m_Border = { 0xA0, 0xA0, 0xA0, 0xFF };
		m_CellEmpty = { 0x00, 0x00, 0x00, 0xFF };
		m_CellSnake = { 0xFF, 0xFF, 0xFF, 0xFF };
		m_CellFood = { 0xA0, 0xA0, 0xA0, 0xFF };

		m_CellDemo = { 0x80, 0x80, 0x80, 0xFF };
	}

	ColorSchemeName m_Name;

	SDL_Color m_Bg;

	SDL_Color m_Text;
	SDL_Color m_ButtonSelector;
	SDL_Color m_DialogBox;

	SDL_Color m_Border;
	SDL_Color m_CellEmpty;
	SDL_Color m_CellSnake;
	SDL_Color m_CellFood;

	SDL_Color m_CellDemo;
};

namespace Globals
{
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
	extern ColorScheme COLOR_SCHEME;

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
