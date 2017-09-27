#pragma once
#include "uiLayout.h"
#include "uiWidget.h"

enum InGameUILabel
{
	IN_GAME_UI_LABEL_DIE_MESSAGE = 0,
	IN_GAME_UI_LABEL_TRY_AGAIN,
	IN_GAME_UI_LABEL_TOTAL
};

enum InGameUIButton
{
	IN_GAME_UI_BUTTON_YES = 0,
	IN_GAME_UI_BUTTON_NO,
	IN_GAME_UI_BUTTON_TOTAL
};

class GameOverLayout : public UILayout {
public:
	GameOverLayout() :
		m_SnakeDied(false),
		m_SelectedButton(IN_GAME_UI_BUTTON_YES)
	{};

	UILayout* HandleInput(SDL_Event *event, GameScreen **newScreen);
	GameEvent Update(uint32_t elapsed);
	void Render(SDL_Renderer *renderer);

	bool CreateLayout(SDL_Renderer *renderer);
	void DestroyLayout();

	void SnakeDied(int score);

private:
	SDL_Rect m_DialogFrame;
	UILabel m_UILabel[IN_GAME_UI_LABEL_TOTAL];
	UIButton m_UIButton[IN_GAME_UI_BUTTON_TOTAL];

	InGameUIButton m_SelectedButton;
	bool m_SnakeDied;
};