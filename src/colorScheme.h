#pragma once
#include <SDL.h>

enum ColorSchemeName
{
	COLOR_SCHEME_CLASSIC_GREY = 0,
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