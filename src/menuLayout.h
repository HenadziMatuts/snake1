#pragma once
#include "uiLayout.h"
#include <vector>

enum MenuInvites
{
	MENU_INVITE_START = 0,
	MENU_INVITE_SETTINGS,
	MENU_INVITE_EXIT,
	MENU_INVITE_RESUME,
	MENU_INVITE_RESTART,
	MENU_INVITE_CREDITS
};

enum MenuAction
{
	MENU_ACTION_NONE = 0,
	MENU_ACTION_START_GAME,
	MENU_ACTION_RESUME_GAME,
	MENU_ACTION_SETTINGS,
	MENU_ACTION_QUIT_GAME,
};

class MenuLayout : public UILayout {
public:
	MenuLayout() :
		m_Timer(0),
		m_IsInviteVisible(false),
		m_IsPaused(false),
		m_Quit(false)
	{};

	UILayout* HandleEvents(SDL_Event *event, GameScreen **newScreen);
	GameEvent Update(uint32_t elapsed);
	void Render(SDL_Renderer *renderer);

	bool CreateLayout(SDL_Renderer *renderer);
	void DestroyLayout();

	/* true - set, false unset */
	void SetPauseLayout(bool set);

private:
	/* graphics */
	SDL_Texture *m_Title, *m_Invite[6];

	/* active invites */
	bool m_IsPaused;
	std::vector<MenuInvites> m_ActiveInvites;
	int m_CurrentActiveIndex;

	/* animation timing */
	uint32_t m_Timer;
	bool m_IsInviteVisible;

	/* are we quitting? */
	bool m_Quit;

	void ResetTimer();
	MenuAction HandleInput(SDL_Event *event);
};