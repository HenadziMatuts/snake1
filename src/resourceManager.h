#pragma once
#include "colorScheme.h"
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
	 *
	 */
	ColorScheme* FindColorScheme(char *name);

private:
	bool LoadFont();

	bool LoadColorSchemes();
	void AddColorScheme(ColorScheme *newScheme);
	bool IsUniqueColorSchemeName(char *name);
	void DestroyColorSchemes();

	TTF_Font *m_Font;
	ColorScheme *m_ColorSchemes;
};