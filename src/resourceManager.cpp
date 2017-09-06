#include "resourceManager.h"
#include "utilities.h"

bool ResourceManager::LoadResources()
{
	m_Font = TTF_OpenFont("assets\\FFFFORWA.TTF", 60);
	if (!m_Font)
	{
		LOG_ERR("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
		return false;
	}
	return true;
}

void ResourceManager::UnloadResources()
{
	TTF_CloseFont(m_Font);
}

TTF_Font* ResourceManager::GetFont()
{
	return m_Font;
}

TTF_Font* ResourceManager::GetFont(int ptsize)
{
	TTF_Font *font = TTF_OpenFont("assets\\FFFFORWA.TTF", ptsize);
	if (!font)
	{
		LOG_ERR("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
		return false;
	}
	return font;
}

void ResourceManager::DestroyFont(TTF_Font *font)
{
	TTF_CloseFont(font);
}

