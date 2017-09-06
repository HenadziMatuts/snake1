#include "game.h"
#include "utilities.h"
#include "globals.h"
#include <cstdio>
#include <cstdint>

void ExitCallback()
{
	exit(1);
}

Game& Game::Instance()
{
	static Game *instance = new Game();
	return *instance;
}

ResourceManager& Game::Resources()
{
	return m_ResourceManager;
}

void Game::Initialize()
{
	LOG_INFO("Initializing SDL...");
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG_FATAL("SDL could not initialize! SDL_Error: %s", SDL_GetError());
		CRASH(ExitCallback);
	}
	LOG_INFO("Initializing SDL...OK");

	/* Set texture filtering to linear */
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
	{
		LOG_WARN("Linear texture filtering not enabled!");
	}

	LOG_INFO("Creating window...");
	m_Window = SDL_CreateWindow("snake!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
					Globals::SCREEN_WIDTH, Globals::SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (!m_Window)
	{
		LOG_FATAL("Window could not be created! SDL_Error: %s", SDL_GetError());
		CRASH(ExitCallback);
	}
	LOG_INFO("Creating window...OK");

	LOG_INFO("Creating renderer...");
	m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!m_Renderer)
	{
		LOG_FATAL("Renderer could not be created! SDL Error: %s", SDL_GetError());
		CRASH(ExitCallback);
	}
	LOG_INFO("Creating renderer...OK");

	/* Initialize renderer */
	SDL_SetRenderDrawBlendMode(m_Renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(m_Renderer, 0xFF, 0xFF, 0xFF, 0xFF);

	LOG_INFO("Initializing SDL_ttf...");
	if (TTF_Init() == -1)
	{
		LOG_FATAL("SDL_ttf could not initialize! SDL_ttf Error: %s", TTF_GetError());
		CRASH(ExitCallback);
	}
	LOG_INFO("Initializing SDL_ttf...OK");

	LOG_INFO("Loading resources...");
	if (!m_ResourceManager.LoadResources())
	{
		LOG_FATAL("Unable to load resources");
		CRASH(ExitCallback);
	}
	LOG_INFO("Loading resources...OK");

	LOG_INFO("Building UI...");
	if (!Globals::menuLayout.CreateLayout(m_Renderer)
		|| !Globals::inGameLayout.CreateLayout(m_Renderer)
		|| !Globals::settingsLayout.CreateLayout(m_Renderer)
		|| !Globals::newGameLayout.CreateLayout(m_Renderer))
	{
		LOG_FATAL("Unable to build UI");
		CRASH(ExitCallback);
	}
	LOG_INFO("Building UI...OK");

	/* initializing stuff */
	srand((unsigned int)time(nullptr));
}

int Game::Run()
{
	bool quit = false;
	GameScreen *currentScreen = &Globals::menuScreen, *newScreen = nullptr;
	SDL_Event event;
	uint32_t prev = SDL_GetTicks(), curr, elapsed;

	LOG_INFO("Snake! game started");
	
	/* game loop */
	while (!quit)
	{
		curr = SDL_GetTicks();
		elapsed = curr - prev;
		prev = curr;

		/* handle input */
		while (SDL_PollEvent(&event) != 0)
		{
			switch (event.type)
			{
			case SDL_QUIT:
				quit = true;
				break;

			default:
				newScreen = currentScreen->HandleEvents(&event);
				if (newScreen)
				{
					currentScreen = newScreen;
				}
				break;
			}
		}

		/* update state */
		switch (currentScreen->Update(elapsed))
		{
			case GAME_EVENT_QUIT:
				quit = true;
				break;

			default:
				break;
		}

		/* render */
		Render(currentScreen);
	}

	return Quit();
}

SDL_Renderer* Game::GetRenderer()
{
	return m_Renderer;
}

void Game::Render(GameScreen *screen)
{
	SDL_SetRenderDrawColor(m_Renderer, 0xE0, 0xE0, 0xE0, 0xFF);
	SDL_RenderClear(m_Renderer);

	screen->Render(m_Renderer);

	SDL_RenderPresent(m_Renderer);
}

int Game::Quit()
{
	/* Unload resources */
	m_ResourceManager.UnloadResources();

	/* Destroy layouts */
	Globals::menuLayout.DestroyLayout();
	Globals::inGameLayout.DestroyLayout();
	Globals::settingsLayout.DestroyLayout();
	Globals::newGameLayout.DestroyLayout();

	/* Destroy window and renderer */
	SDL_DestroyRenderer(m_Renderer);
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	m_Renderer = nullptr;

	/* Quit SDL subsystems */
	SDL_Quit();
	TTF_Quit();

	return 0;
}
