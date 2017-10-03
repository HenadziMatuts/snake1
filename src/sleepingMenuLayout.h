#pragma once
#include "uiLayout.h"
#include "uiWidget.h"

enum SleepingMenuUILabel
{
	SLEEPING_MENU_UI_TITLE = 0,
	SLEEPING_MENU_UI_START,
	SLEEPING_MENU_UI_SETTINGS,
	SLEEPING_MENU_UI_EXIT,
	SLEEPING_MENU_UI_RESUME,
	SLEEPING_MENU_UI_RESTART,
	SLEEPING_MENU_UI_AUTHOR,
	SLEEPING_MENU_UI_TOTAL
};

enum SleepingMenuAction
{
	SLEEPING_MENU_ACTION_NONE = 0,
	SLEEPING_MENU_ACTION_START_GAME,
	SLEEPING_MENU_ACTION_RESUME_GAME,
	SLEEPING_MENU_ACTION_SETTINGS,
	SLEEPING_MENU_ACTION_QUIT_GAME,
	SLEEPING_MENU_ACTION_WAKE_UP,
};

class SleepingMenuLayout : public UILayout
{
public:
	SleepingMenuLayout() :
		m_Timer(0),
		m_IsInviteVisible(false),
		m_IsPaused(false),
		m_Quit(false)
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
	UILabel m_UILabel[SLEEPING_MENU_UI_TOTAL];

	/* active invites */
	bool m_IsPaused;
	std::vector<SleepingMenuUILabel> m_ActiveInvites;
	int m_VisibleActiveInviteIndex;

	/* animation timing */
	uint32_t m_Timer;
	bool m_IsInviteVisible;

	/* are we quitting? */
	bool m_Quit;

	SleepingMenuAction ProcessInput(SDL_Event *event);
	void ResetTimer();

};