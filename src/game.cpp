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

EventBus& Game::Events()
{
	return m_EventBus;
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

	LOG_INFO("Creating window...");
	SDL_DisplayMode target, matching;
	target.w = Globals::SCREEN_WIDTH;
	target.h = Globals::SCREEN_HEIGHT;
	target.format = 0;
	target.refresh_rate = 0;
	target.driverdata = 0;

	if (!SDL_GetClosestDisplayMode(0, &target, &matching))
	{
		LOG_FATAL("Can't find suitable display mode");
		CRASH(ExitCallback);
	}
	Globals::SCREEN_WIDTH = matching.w;
	Globals::SCREEN_HEIGHT = matching.h;

	uint32_t flags = SDL_WINDOW_SHOWN;
	flags |= (Globals::FULLSCREEN) ? SDL_WINDOW_FULLSCREEN : 0;
	m_Window = SDL_CreateWindow("snake!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
					Globals::SCREEN_WIDTH, Globals::SCREEN_HEIGHT, flags);
	if (!m_Window)
	{
		LOG_FATAL("Window could not be created! SDL_Error: %s", SDL_GetError());
		CRASH(ExitCallback);
	}
	LOG_INFO("Creating window...OK");

	int n = SDL_GetNumDisplayModes(0);
	for (int i = n - 1; i >= 0; i--)
	{
		SDL_DisplayMode mode;
		SDL_GetDisplayMode(0, i, &mode);
		m_DisplayModes.push_back(mode);
	}

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
	if (!RebuidUI())
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
					newScreen = currentScreen->HandleInput(&event);
					if (newScreen)
					{
						currentScreen = newScreen;
					}
					break;
			}
		}

		/* update state */
		currentScreen->Update(elapsed, &m_EventBus);

		/* handling events */
		std::vector<GameEvent> *gameEvents = m_EventBus.GameEvents();
		size_t n = gameEvents->size();

		for (size_t i = 0; i < n; i++)
		{
			switch ((*gameEvents)[i])
			{
				case GAME_EVENT_QUIT:
					quit = true;
					break;
			}
		}

		m_EventBus.Proceed();

		/* render */
		Render(currentScreen);
	}

	return Quit();
}

bool Game::RebuidUI()
{
	Globals::WINDOW_SCALE_FACTOR = (float)Globals::SCREEN_HEIGHT / 600;
	Globals::ASPECT_RATIO =
		(((float)Globals::SCREEN_WIDTH / Globals::SCREEN_HEIGHT) > 1.3f
			&& ((float)Globals::SCREEN_WIDTH / Globals::SCREEN_HEIGHT) < 1.4f) ?
		ASPECT_RATIO_4_3 : ASPECT_RATIO_16_9;

	if (!Globals::menuLayout.CreateLayout(m_Renderer)
		|| !Globals::inGameLayout.CreateLayout(m_Renderer)
		|| !Globals::settingsLayout.CreateLayout(m_Renderer)
		|| !Globals::newGameLayout.CreateLayout(m_Renderer))
	{
		return false;
	}

	return true;
}

void Game::SetFullscreen(bool fullscreen)
{
	SDL_SetWindowFullscreen(m_Window, fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
	RebuidUI();
}

void Game::ChangeResolution(int width, int height, bool fullscreen)
{
	size_t index = 0;

	for (index; index < m_DisplayModes.size(); index++)
	{
		if (m_DisplayModes[index].w == width
			&& m_DisplayModes[index].h == height)
		{
			break;
		}
	}

	if (fullscreen)
	{
		SDL_SetWindowDisplayMode(m_Window, &m_DisplayModes[index]);
	}
	SDL_SetWindowSize(m_Window, m_DisplayModes[index].w, m_DisplayModes[index].h);

	RebuidUI();
}

void Game::GetNextResolution(int currentW, int currentH, int *nextW, int *nextH, bool prev)
{
	size_t index = 0;

	for (index; index < m_DisplayModes.size(); index++)
	{
		if (m_DisplayModes[index].w == currentW
			&& m_DisplayModes[index].h == currentH)
		{
			break;
		}
	}

	index = Utilities::ModuloSum(index, prev ? -1 : 1, m_DisplayModes.size());
	*nextW = m_DisplayModes[index].w;
	*nextH = m_DisplayModes[index].h;
}

void Game::Render(GameScreen *screen)
{
	SDL_Color bg = Globals::COLOR_SCHEME->m_Bg;

	SDL_SetRenderDrawColor(m_Renderer, bg.r, bg.g, bg.b, bg.a);
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
