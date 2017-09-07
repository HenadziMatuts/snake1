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
	SETTINGS_UI_LABEL_SCHEME_GREY,
	SETTINGS_UI_LABEL_SCHEME_MOON,
	SETTINGS_UI_LABEL_TOTAL
};

enum SettingsUIButton
{
	SETTINGS_UI_BUTTON_BACK = 0,
	SETTINGS_UI_BUTTON_MOVEMENT,
	SETTINGS_UI_BUTTON_COLOR_SCHEME,
	SETTINGS_UI_BUTTON_TOTAL
};

struct GameSettings
{
	GameSettings() :
		m_SmoothMovement(true),
		m_ColorScheme(COLOR_SCHEME_CLASSIC_GREY)
	{};

	bool m_SmoothMovement;
	ColorSchemeName m_ColorScheme;
};

class SettingsLayout : public UILayout {
public:
	SettingsLayout() :
		m_SelectedButton(SETTINGS_UI_BUTTON_BACK)
	{};

	void Enter();	
	UILayout* HandleEvents(SDL_Event *event, GameScreen **newScreen);
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