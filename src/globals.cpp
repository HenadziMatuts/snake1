#include "globals.h"
#include <ctime>

FILE *Globals::LOG = stderr;
ColorScheme Globals::defaultColorScheme = {
	"classic grey",
	{ 0xE0, 0xE0, 0xE0, 0xFF },
	{ 0x00, 0x00, 0x00, 0xFF },
	{ 0x90, 0x90, 0x90, 0x80 },
	{ 0xC0, 0xC0, 0xC0, 0xF0 },
	{ 0xA0, 0xA0, 0xA0, 0xFF },
	{ 0x00, 0x00, 0x00, 0xFF },
	{ 0xFF, 0xFF, 0xFF, 0xFF },
	{ 0xA0, 0xA0, 0xA0, 0xFF },
	{ 0x80, 0x80, 0x80, 0xFF },
	&Globals::defaultColorScheme,
	&Globals::defaultColorScheme,
	&Globals::defaultColorScheme
};
char Globals::ASCII_TIME[128] = "\0";

int Globals::SCREEN_WIDTH = 800;
int Globals::SCREEN_HEIGHT = 600;
float Globals::WINDOW_SCALE_FACTOR = (float)Globals::SCREEN_HEIGHT / 600;

GameMode Globals::MODE = GAME_MODE_TRAINING;
uint32_t Globals::GRID_DIMENSION = 40;
int Globals::GAME_SPEED = 10;
int Globals::BODY_SIZE = 10;
bool Globals::BORDERLESS = true;

bool Globals::SMOOTH_MOVEMENT = true;
ColorScheme *Globals::COLOR_SCHEME = &Globals::defaultColorScheme;

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

char* Globals::ChangeColorScheme(bool prev)
{
	COLOR_SCHEME = (prev) ? COLOR_SCHEME->m_PrevScheme :
		COLOR_SCHEME->m_NextScheme;

	return COLOR_SCHEME->m_Name;
}

void Globals::AddColorScheme(ColorScheme *scheme)
{
	ColorScheme *tail = COLOR_SCHEME;
	ColorScheme *head = COLOR_SCHEME->m_Head;

	while (tail->m_NextScheme != head)
	{
		tail = tail->m_NextScheme;
	}

	tail->m_NextScheme = scheme;
	head->m_PrevScheme = scheme;

	scheme->m_PrevScheme = tail;
	scheme->m_NextScheme =
		scheme->m_Head = head;
}

bool Globals::IsUniqueColorSchemeName(char *name)
{
	ColorScheme *scheme = COLOR_SCHEME->m_Head;

	do
	{
		if (!strcmp(scheme->m_Name, name))
		{
			return false;
		}

		scheme = scheme->m_NextScheme;

	} while (scheme->m_NextScheme != scheme->m_Head);

	return strcmp(scheme->m_Name, name) ? true : false;
}

void Globals::DestroyColorSchemes()
{
	ColorScheme *head = COLOR_SCHEME->m_Head;
	ColorScheme *scheme = head->m_NextScheme;

	while (scheme->m_NextScheme != head)
	{
		scheme = scheme->m_NextScheme;
		delete scheme->m_PrevScheme;
		scheme->m_PrevScheme = nullptr;
	}

	delete scheme;
	scheme = nullptr;
}
