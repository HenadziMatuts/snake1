#pragma once
#include "uiLayout.h"
#include "uiWidget.h"
#include "colorScheme.h"
#include <vector>
#include <SDL.h>

enum SettingsUILabel
{
	SETTINGS_UI_LABEL_TITLE = 0,
	SETTINGS_UI_LABEL_SMOOTH,
	SETTINGS_UI_LABEL_DISCRETE,
	SETTINGS_UI_LABEL_COLOR_SCHEME,
	SETTINGS_UI_LABEL_RESOLUTION,
	SETTINGS_UI_LABEL_FULLSCREEN,
	SETTINGS_UI_LABEL_TOTAL
};

enum SettingsUIButton
{
	SETTINGS_UI_BUTTON_BACK = 0,
	SETTINGS_UI_BUTTON_APPLY,
	SETTINGS_UI_BUTTON_MOVEMENT,
	SETTINGS_UI_BUTTON_COLOR_SCHEME,
	SETTINGS_UI_BUTTON_RESOLUTION,
	SETTINGS_UI_BUTTON_FULLSCREEN,
	SETTINGS_UI_BUTTON_TOTAL
};

struct GameSettings
{
	GameSettings() :
		m_SmoothMovement(true),
		m_ColorScheme("classic grey"),
		m_ResolutionWidth(1366),
		m_ResolutionHeight(768),
		m_Fullscreen(false)
	{};

	bool m_SmoothMovement;
	char *m_ColorScheme;
	
	int m_ResolutionWidth, m_ResolutionHeight;
	bool m_Fullscreen;
};

class SettingsLayout : public UILayout {
public:
	SettingsLayout() :
		m_SelectedButton(SETTINGS_UI_BUTTON_BACK)
	{};

	void Enter();	
	UILayout* HandleInput(SDL_Event *event, GameScreen **newScreen);
	GameEvent Update(uint32_t elapsed);
	void Render(SDL_Renderer *renderer);

	bool CreateLayout(SDL_Renderer *renderer);
	void DestroyLayout();

private:
	UILabel m_UILabel[SETTINGS_UI_LABEL_TOTAL];
	UIButton m_UIButton[SETTINGS_UI_BUTTON_TOTAL];

	SettingsUIButton m_SelectedButton;

	GameSettings m_Settings;
};