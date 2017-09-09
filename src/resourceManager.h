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
	* Returns font.
	*/
	TTF_Font* GetFont();

private:
	bool LoadFont();
	bool LoadColorSchemes();

	TTF_Font *m_Font;
};