#pragma once
#include "uiLayout.h"

const unsigned int DIE_MESSAGES_TOTAL = 13;
const unsigned int AFFRONTS = 0;
const unsigned int NEUTRALS = 5;
const unsigned int CONGRATZ = 10;

class InGameLayout : public UILayout {
public:
	InGameLayout() :
		m_SnakeDied(false),
		m_IsTryAgainSelected(true)
	{};

	UILayout* HandleEvents(SDL_Event *event, GameScreen **newScreen);
	GameEvent Update(uint32_t elapsed);
	void Render(SDL_Renderer *renderer);

	bool CreateLayout(SDL_Renderer *renderer);
	void DestroyLayout();

	void SnakeDied(int score);

private:
	bool m_SnakeDied;
	bool m_IsTryAgainSelected;

	/* drawables */
	SDL_Texture *m_TryAgain;
	SDL_Texture *m_Yes, *m_No;
	SDL_Texture *m_DieMessages[DIE_MESSAGES_TOTAL];

	SDL_Texture *m_CurrentMessage;
};