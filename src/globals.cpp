#include "globals.h"
#include <ctime>

FILE *Globals::LOG = stderr;
char Globals::ASCII_TIME[128] = "\0";

int Globals::SCREEN_WIDTH = 1366;
int Globals::SCREEN_HEIGHT = 768;

float Globals::WINDOW_SCALE_FACTOR =
	(float)Globals::SCREEN_HEIGHT / 600;

AspectRatio Globals::ASPECT_RATIO =
	(((float)Globals::SCREEN_WIDTH / Globals::SCREEN_HEIGHT) > 1.3f
	&& ((float)Globals::SCREEN_WIDTH / Globals::SCREEN_HEIGHT) < 1.4f) ?
	ASPECT_RATIO_4_3 : ASPECT_RATIO_16_9;

bool Globals::FULLSCREEN = false;

GameMode Globals::MODE = GAME_MODE_TRAINING;
uint32_t Globals::GRID_DIMENSION = 40;
int Globals::GAME_SPEED = 10;
int Globals::BODY_SIZE = 10;
bool Globals::BORDERLESS = true;

bool Globals::SMOOTH_MOVEMENT = true;
ColorScheme *Globals::COLOR_SCHEME = &DEFAULT_COLOR_SCHEME;

MenuScreen Globals::menuScreen;
InGameScreen Globals::inGameScreen;

MenuLayout Globals::menuLayout;
SleepingMenuLayout Globals::sleepingMenuLayout;
InGameLayout Globals::inGameLayout;
GameOverLayout Globals::gameOverLayout;
SettingsLayout Globals::settingsLayout;
NewGameLayout Globals::newGameLayout;
ProfileLayout Globals::profileLayout;

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