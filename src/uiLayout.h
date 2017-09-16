#pragma once
#include "gameScreen.h"
#include <SDL.h>

class UILayout
{
public:
	virtual ~UILayout() {};

	virtual UILayout* HandleInput(SDL_Event *event, GameScreen **newScreen) = 0;
	virtual GameEvent Update(uint32_t elapsed) = 0;
	virtual void Render(SDL_Renderer *renderer) = 0;

	virtual bool CreateLayout(SDL_Renderer *renderer) = 0;
	virtual void DestroyLayout() = 0;
};