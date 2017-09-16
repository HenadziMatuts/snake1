#pragma once
#include "uiLayout.h"
#include "uiWidget.h"
#include <vector>

enum NewGameUILabel
{
	NEW_GAME_UI_LABEL_TITLE = 0,

	NEW_GAME_UI_LABEL_TRAINING,
	NEW_GAME_UI_LABEL_SURVIVAL,
	
	NEW_GAME_UI_LABEL_FIELD_SIZE,
	NEW_GAME_UI_LABEL_SPEED,
	NEW_GAME_UI_LABEL_BODY_SIZE,
	
	NEW_GAME_UI_LABEL_BORDERLESS,
	NEW_GAME_UI_LABEL_NOT_BORDERLESS,

	NEW_GAME_UI_LABEL_TOTAL
};

enum NewGameUIButton
{
	NEW_GAME_UI_BUTTON_MODE,
	NEW_GAME_UI_BUTTON_FIELD_SIZE,
	NEW_GAME_UI_BUTTON_SPEED,
	NEW_GAME_UI_BUTTON_BODY_SIZE,
	NEW_GAME_UI_BUTTON_IS_BORDERLESS,

	NEW_GAME_UI_BUTTON_START,
	NEW_GAME_UI_BUTTON_BACK,

	NEW_GAME_UI_BUTTON_TOTAL
};

struct NewGameSettings
{
	NewGameSettings() :
		m_Mode(GAME_MODE_TRAINING),
		m_GridDimension(40),
		m_GameSpeed(10),
		m_BodySize(10),
		m_IsBorderless(true)
	{};

	GameMode m_Mode;
	uint32_t m_GridDimension;
	int m_GameSpeed;
	int m_BodySize;
	bool m_IsBorderless;
};

class NewGameLayout : public UILayout {
public:
	NewGameLayout() :
		m_SelectedButton(NEW_GAME_UI_BUTTON_START)
	{};

	void Enter();

	UILayout* HandleInput(SDL_Event *event, GameScreen **newScreen);
	GameEvent Update(uint32_t elapsed);
	void Render(SDL_Renderer *renderer);

	bool CreateLayout(SDL_Renderer *renderer);
	void DestroyLayout();

private:
	UILabel m_UILabel[NEW_GAME_UI_LABEL_TOTAL];
	UIButton m_UIButton[NEW_GAME_UI_BUTTON_TOTAL];

	NewGameUIButton m_SelectedButton;
	NewGameSettings m_Settings;
};