#include "eventBus.h"

void EventBus::PostGameEvent(GameEvent gameEvent)
{
	m_GameEvents.push_back(gameEvent);
};

void EventBus::PostInGameEvent(InGameEvent inGameEvent)
{
	m_InGameEvents.push_back(inGameEvent);
}

std::vector<GameEvent>* EventBus::GameEvents()
{
	return &m_GameEvents;
}

std::vector<InGameEvent>* EventBus::InGameEvents()
{
	return &m_InGameEvents;
}

void EventBus::ClearEvents()
{
	m_GameEvents.clear();
	m_InGameEvents.clear();
}