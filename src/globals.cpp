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