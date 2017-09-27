#pragma once
#include "uiLayout.h"
#include "uiWidget.h"

class InGameLayout : public UILayout {
public:
	
	UILayout* HandleInput(SDL_Event *event, GameScreen **newScreen);
	GameEvent Update(uint32_t elapsed);
	void Render(SDL_Renderer *renderer);

	bool CreateLayout(SDL_Renderer *renderer);
	void DestroyLayout();
};