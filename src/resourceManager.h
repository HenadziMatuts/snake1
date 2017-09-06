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

	/**
	 * Returns font.
	 */
	TTF_Font* GetFont(int ptsize);

	/**
	 *
	 */
	void DestroyFont(TTF_Font *font);

private:
	TTF_Font *m_Font;
};