#include "globals.h"
#include <ctime>

int Globals::SCREEN_WIDTH = 800;
int Globals::SCREEN_HEIGHT = 600;
float Globals::WINDOW_SCALE_FACTOR = (float)Globals::SCREEN_HEIGHT / 600;

GameMode Globals::MODE = GAME_MODE_TRAINING;
uint32_t Globals::GRID_DIMENSION = 40;
int Globals::GAME_SPEED = 10;
int Globals::BODY_SIZE = 10;
bool Globals::BORDERLESS = true;

bool Globals::SMOOTH_MOVEMENT = true;
ColorScheme Globals::COLOR_SCHEME;

FILE *Globals::LOG = stderr;

char Globals::ASCII_TIME[128] = "\0";

MenuScreen Globals::menuScreen;
InGameScreen Globals::inGameScreen;

MenuLayout Globals::menuLayout;
InGameLayout Globals::inGameLayout;
SettingsLayout Globals::settingsLayout;
NewGameLayout Globals::newGameLayout;

char* Globals::Time()
{
	time_t t = time(NULL);
	struct tm _t;

	localtime_s(&_t, &t);
	strftime(ASCII_TIME, sizeof(ASCII_TIME), "%F %T", &_t);

	return ASCII_TIME;
}

void Globals::ChangeColorScheme(ColorSchemeName name)
{
	switch (name)
	{
	case COLOR_SCHEME_CLASSIC_GREY:
		COLOR_SCHEME.m_Name = COLOR_SCHEME_CLASSIC_GREY;

		COLOR_SCHEME.m_Bg = { 0xE0, 0xE0, 0xE0, 0xFF };
		COLOR_SCHEME.m_Text = { 0x00, 0x00, 0x00, 0xFF };
		COLOR_SCHEME.m_ButtonSelector = { 0x90, 0x90, 0x90, 0x80 };
		COLOR_SCHEME.m_DialogBox = { 0xC0, 0xC0, 0xC0, 0xFF };
		COLOR_SCHEME.m_Border = { 0xA0, 0xA0, 0xA0, 0xFF };
		COLOR_SCHEME.m_CellEmpty = { 0x00, 0x00, 0x00, 0xFF };
		COLOR_SCHEME.m_CellSnake = { 0xFF, 0xFF, 0xFF, 0xFF };
		COLOR_SCHEME.m_CellFood = { 0xA0, 0xA0, 0xA0, 0xFF };
		COLOR_SCHEME.m_CellDemo = { 0x80, 0x80, 0x80, 0xFF };
		break;

	case COLOR_SCHEME_BLUE_MOON:
		COLOR_SCHEME.m_Name = COLOR_SCHEME_BLUE_MOON;

		COLOR_SCHEME.m_Bg = { 0x00, 0x00, 0x10, 0xFF };
		COLOR_SCHEME.m_Text = { 0x00, 0x00, 0xA0, 0xFF };
		COLOR_SCHEME.m_ButtonSelector = { 0xFF, 0xFF, 0xFF, 0x80 };
		COLOR_SCHEME.m_DialogBox = { 0x00, 0x00, 0x10, 0xF0 };
		COLOR_SCHEME.m_Border = { 0xFF, 0xFF, 0xFF, 0xFF };
		COLOR_SCHEME.m_CellEmpty = { 0x72, 0x6f, 0xa5, 0xFF };
		COLOR_SCHEME.m_CellSnake = { 0x00, 0x00, 0x90, 0xFF };
		COLOR_SCHEME.m_CellFood = { 0x20, 0x00, 0xf0, 0xFF };
		COLOR_SCHEME.m_CellDemo = { 0x50, 0x50, 0xff, 0xFF };
		break;

	default:
		break;
	}
}