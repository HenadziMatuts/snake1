#pragma once
#include <SDL_ttf.h>

class ResourceManager
{
public:
	/**
	 * Load media and other resources.
	 */
	bool LoadResources();

	/**
	 * Unload recources.
	 */
	void UnloadResources();

	/**
	 * Returns loaded font.
	 */
	TTF_Font* GetFont();

private:
	TTF_Font *m_Font;
};