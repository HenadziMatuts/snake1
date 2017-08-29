#include "resourceManager.h"
#include "utilities.h"

bool ResourceManager::LoadResources()
{
	LOG_INFO("Loading fonts...");
	m_Font = TTF_OpenFont("assets\\FFFFORWA.TTF", 56);
	if (!m_Font)
	{
		LOG_ERR("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
		return false;
	}
	LOG_INFO("Loading fonts...OK");

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

