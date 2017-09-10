#pragma once
#include <SDL.h>

struct ColorScheme
{
	char m_Name[32];

	SDL_Color m_Bg;
	SDL_Color m_Text;
	SDL_Color m_ButtonSelector;
	SDL_Color m_DialogBox;
	SDL_Color m_Border;
	SDL_Color m_CellEmpty;
	SDL_Color m_CellSnake;
	SDL_Color m_CellFood;
	SDL_Color m_CellDemo;

	ColorScheme *m_Head;
	ColorScheme *m_NextScheme, *m_PrevScheme;
};

extern ColorScheme DEFAULT_COLOR_SCHEME;