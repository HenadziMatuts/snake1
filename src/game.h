#pragma once
#include "resourceManager.h"
#include "gameScreen.h"
#include <SDL.h>
#include <SDL_ttf.h>

/**
 * Main Snake game class.
 */
class Game {
public:
	static Game& Instance();
	ResourceManager& Resources();

	/**
	 * Initialize subsystems.
	 */
	void Initialize();

	/**
	 * Starts game loop.
	 */
	int Run();

	SDL_Renderer* GetRenderer();

private:
	Game() :
		m_Window(nullptr),
		m_Renderer(nullptr)
	{};

	/* rendering subsystem */
	SDL_Window *m_Window;
	SDL_Renderer *m_Renderer;

	/* resources */
	ResourceManager m_ResourceManager;

	/**
	 *
	 */
	void Render(GameScreen *screen);

	/**
	 * Deinitilize subsystems and quit.
	 */
	int Quit();
};