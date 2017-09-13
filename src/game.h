#pragma once
#include "resourceManager.h"
#include "gameScreen.h"
#include "eventBus.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>

/**
 * Main Snake game class.
 */
class Game {
public:
	static Game& Instance();
	ResourceManager& Resources();
	EventBus& Events();

	/**
	 * Initialize subsystems.
	 */
	void Initialize();

	/**
	 * Starts game loop.
	 */
	int Run();

	/**
	 *
	 */
	bool RebuidUI();

	/**
	 *
	 */
	void SetFullscreen(bool fullscreen);

	/**
	 *
	 */
	void ChangeResolution(int width, int height, bool fullscreen);

	/**
	 *
	 */
	void GetNextResolution(int currentW, int currentH, int *nextW, int *nextH, bool prev);

private:
	Game() :
		m_Window(nullptr),
		m_Renderer(nullptr)
	{};

	/* rendering subsystem */
	SDL_Window *m_Window;
	SDL_Renderer *m_Renderer;
	std::vector<SDL_DisplayMode> m_DisplayModes;

	/* resources */
	ResourceManager m_ResourceManager;

	/* event bus */
	EventBus m_EventBus;

	/**
	 *
	 */
	void Render(GameScreen *screen);

	/**
	 * Deinitilize subsystems and quit.
	 */
	int Quit();
};