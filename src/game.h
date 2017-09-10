#pragma once
#include "resourceManager.h"
#include "gameScreen.h"
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
	void SetFullscreen(bool foolscreen);

	/**
	 *
	 */
	void ChangeResolution(int width, int height);

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

	/* resources */
	ResourceManager m_ResourceManager;

	std::vector<SDL_DisplayMode> m_DisplayModes;

	/**
	 *
	 */
	void Render(GameScreen *screen);

	/**
	 * Deinitilize subsystems and quit.
	 */
	int Quit();
};