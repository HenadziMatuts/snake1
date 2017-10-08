#pragma once
#include "resourceManager.h"
#include "profileManager.h"
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
	ProfileManager& Profiles();

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
		m_Renderer(nullptr),
		m_FadingTimer(0),
		m_Fading(false),
		m_ScreenSwitched(true)
	{};

	/* rendering subsystem */
	SDL_Window *m_Window;
	SDL_Renderer *m_Renderer;
	std::vector<SDL_DisplayMode> m_DisplayModes;

	/* resources */
	ResourceManager m_ResourceManager;

	/* profiles */
	ProfileManager m_ProfileManager;

	/* event bus */
	EventBus m_EventBus;

	/**
	 *
	 */
	void Render(GameScreen *screen);

	/**
	 *
	 */
	bool ReadSettings();

	/**
	 *
	 */
	bool StoreSettings();

	/**
	 * Deinitilize subsystems and quit.
	 */
	int Quit();

	/* screems crossfading */
	bool m_Fading;
	uint32_t m_FadingTimer;

	GameScreen *m_NewScreen;
	bool m_ScreenSwitched;
};