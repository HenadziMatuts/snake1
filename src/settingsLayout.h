#pragma once
#include "uiLayout.h"
#include <SDL.h>

enum SettingsInteractable
{
	SETTINGS_SMOOTH_MOVEMENT = 0,
	SETTINGS_BACK,
	SETTINGS_TOTAL
};

class SettingsLayout : public UILayout {
public:
	SettingsLayout() :
		m_SmoothMovement(true)
	{};

	void Enter();

	UILayout* HandleEvents(SDL_Event *event, GameScreen **newScreen);
	GameEvent Update(uint32_t elapsed);
	void Render(SDL_Renderer *renderer);

	bool CreateLayout(SDL_Renderer *renderer);
	void DestroyLayout();

private:
	SDL_Texture *m_Title, *m_Smooth, *m_Discrete;
	SDL_Texture *m_Interactables[SETTINGS_TOTAL];

	SettingsInteractable m_Selected;

	/* settings */
	bool m_SmoothMovement;
};