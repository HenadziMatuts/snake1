#pragma once
#include "uiLayout.h"
#include "uiWidget.h"
#include "profileManager.h"

enum ProfileUILabel {
	PROFILE_UI_LABEL_TITLE = 0,
	PROFILE_UI_LABEL_TOTAL
};

enum ProfileUIButton {
	PROFILE_UI_BUTTON_BACK = 0,
	
	PROFILE_UI_BUTTON_CHANGE,
	PROFILE_UI_BUTTON_NEW,
	PROFILE_UI_BUTTON_DELETE,

	PROFILE_UI_BUTTON_TOTAL
};

class ProfileLayout : public UILayout {
public:
	ProfileLayout() :
		m_IsTextInputActive(false)
	{};

	void Enter();

	UILayout* HandleInput(SDL_Event *event, GameScreen **newScreen);
	GameEvent Update(uint32_t elapsed);
	void Render(SDL_Renderer *renderer);

	bool CreateLayout(SDL_Renderer *renderer);
	void DestroyLayout();

private:
	UILabel m_UILabel[PROFILE_UI_LABEL_TOTAL];

	UIButton m_UIButton[PROFILE_UI_BUTTON_TOTAL];
	ProfileUIButton m_SelectedButton;

	bool m_IsTextInputActive;
	char m_Input[MAX_PROFILE_NAME_SIZE];
};