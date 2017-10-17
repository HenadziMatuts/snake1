#pragma once

#include "uiLayout.h"
#include "uiWidget.h"
#include "profileManager.h"

class ProfileSwitchingSubLayout : public UILayout
{
public:
	void Enter();

	UILayout* HandleInput(SDL_Event *event, GameScreen **newScreen);
	GameEvent Update(uint32_t elapsed);
	void Render(SDL_Renderer *renderer);

	bool CreateLayout(SDL_Renderer *renderer);
	void DestroyLayout();

private:
	SDL_Rect m_BoxFrame;
	
	UIButton m_UIButton[TOTAL_PROFILES - 1];

	int m_SelectedButton;
};