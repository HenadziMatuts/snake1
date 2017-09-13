#pragma once
#include "gameField.h"
#include "gameScreen.h"
#include <vector>

class EventBus {
public:
	EventBus()
	{
		m_GameEvents.reserve(32);
		m_InGameEvents.reserve(32);
	}

	void PostGameEvent(GameEvent gameEvent);
	void PostInGameEvent(InGameEvent inGameEvent);

	std::vector<GameEvent>* GameEvents();
	std::vector<InGameEvent>* InGameEvents();

	void ClearEvents();

private:
	std::vector<GameEvent> m_GameEvents;
	std::vector<InGameEvent> m_InGameEvents;
};