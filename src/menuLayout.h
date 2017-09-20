#pragma once
#include "uiLayout.h"
#include "uiWidget.h"
#include <vector>

enum MenuUILabel
{
	MENU_UI_TITLE = 0,
	MENU_UI_INVITE_START,
	MENU_UI_INVITE_SETTINGS,
	MENU_UI_INVITE_EXIT,
	MENU_UI_INVITE_RESUME,
	MENU_UI_INVITE_RESTART,
	MENU_UI_INVITE_CREDITS,
	MENU_UI_TOTAL
};

enum MenuUIButton
{
	MENU_UI_BUTTON_RESUME,
	MENU_UI_BUTTON_NEW_GAME,
	MENU_UI_BUTTON_SETTINGS,
	MENU_UI_BUTTON_PROFILE,
	MENU_UI_BUTTON_EXIT,
	MENU_UI_BUTTON_TOTAL
};

enum MenuAction
{
	MENU_ACTION_NONE = 0,
	MENU_ACTION_START_GAME,
	MENU_ACTION_RESUME_GAME,
	MENU_ACTION_SETTINGS,
	MENU_ACTION_QUIT_GAME,
	MENU_ACTION_WAKE_UP,
};

class MenuLayout : public UILayout {
public:
	MenuLayout() :
		m_Timer(0),
		m_IsInviteVisible(false),
		m_IsPaused(false),
		m_Quit(false),
		m_IsSleeping(true)
	{};

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

	/* active invites */
	bool m_IsPaused;
	std::vector<MenuUILabel> m_ActiveInvites;
	int m_VisibleActiveInviteIndex;

	MenuUIButton m_SelectedButton;

	bool m_IsSleeping;
	uint32_t m_WakeTime;

	/* animation timing */
	uint32_t m_Timer;
	bool m_IsInviteVisible;

	/* are we quitting? */
	bool m_Quit;

	void ResetTimer();
	MenuAction ProcessInput(SDL_Event *event);

	void WakeUp();
	void FallAsleep();
};