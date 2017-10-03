#pragma once
#include "uiLayout.h"
#include "uiWidget.h"
#include <vector>

enum MenuUILabel
{
	MENU_UI_TITLE = 0,
	MENU_UI_TOTAL
};

enum MenuUIButton
{
	MENU_UI_BUTTON_RESUME,
	MENU_UI_BUTTON_NEW_GAME,
	MENU_UI_BUTTON_PROFILE,
	MENU_UI_BUTTON_HIGH_SCORES,
	MENU_UI_BUTTON_SETTINGS,
	MENU_UI_BUTTON_CREDITS,
	MENU_UI_BUTTON_EXIT,
	MENU_UI_BUTTON_TOTAL
};

class MenuLayout : public UILayout {
public:
	MenuLayout() :
		m_Timer(0),
		m_Quit(false),
		m_IsPaused(false)
	{};

	void Enter();

	UILayout* HandleInput(SDL_Event *event, GameScreen **newScreen);
	GameEvent Update(uint32_t elapsed);
	void Render(SDL_Renderer *renderer);

	bool CreateLayout(SDL_Renderer *renderer);
	void DestroyLayout();

	/* true - set, false unset */
	void SetPauseLayout(bool set);

private:
	UILabel m_UILabel[MENU_UI_TOTAL];
	UIButton m_UIButton[MENU_UI_BUTTON_TOTAL];

	MenuUIButton m_SelectedButton;

	bool m_IsTimeToSleep;
	uint32_t m_Timer;

	/* are we quitting? */
	bool m_Quit;

	bool m_IsPaused;
};