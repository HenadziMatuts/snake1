#include "game.h"

int main(int argc, char *argv[])
{
	Game::Instance().Initialize();
	return Game::Instance().Run();
}