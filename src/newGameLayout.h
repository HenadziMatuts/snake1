#pragma once
#include "uiLayout.h"

enum NewGameSettingsInteractable
{
	NEW_GAME_MODE = 0,
	NEW_GAME_FIELD_SIZE,
	NEW_GAME_SPEED,
	NEW_GAME_IS_BORDERLESS,
	NEW_GAME_START,
	NEW_GAME_BACK,
	NEW_GAME_SETTINGS_TOTAL
};

enum NewGameSettingsTip
{
	NEW_GAME_TIPS_MODE = 0,
	NEW_GAME_TIPS_MODE_TRAINING,
	NEW_GAME_TIPS_MODE_SURVIVAL,
	NEW_GAME_TIPS_FIELD_SIZE,
	NEW_GAME_TIPS_SPEED,
	NEW_GAME_TIPS_START,
	NEW_GAME_TIPS_BACK,
	NEW_GAME_TIPS_TOTAL,
};

struct NewGameSettings
{
	NewGameSettings() :
		m_Mode(GAME_MODE_TRAINING),
		m_GridDimension(55),
		m_GameSpeed(20),
		m_IsBorderless(true)
	{};

	GameMode m_Mode;
	uint32_t m_GridDimension;
	int m_GameSpeed;
	bool m_IsBorderless;
};

class NewGameLayout : public UILayout {
public:
	void Enter();

	UILayout* HandleEvents(SDL_Event *event, GameScreen **newScreen);
	GameEvent Update(uint32_t elapsed);
	void Render(SDL_Renderer *renderer);

	bool CreateLayout(SDL_Renderer *renderer);
	void DestroyLayout();

private:
	SDL_Texture *m_Title, *m_Yes, *m_No;
	SDL_Texture *m_Interactables[NEW_GAME_SETTINGS_TOTAL];
	SDL_Texture *m_GameModes[GAME_MODE_TOTAL];
	SDL_Texture *m_Tips[NEW_GAME_TIPS_TOTAL];

	NewGameSettingsInteractable m_Selected;
	NewGameSettingsTip m_Tip;
	bool m_ShowConcreteModeTip;

	NewGameSettings m_Settings;
};