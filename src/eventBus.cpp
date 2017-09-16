#include "eventBus.h"

void EventBus::PostGameEvent(GameEvent gameEvent)
{
	m_GameEvents.push_back(gameEvent);
};

void EventBus::PostInGameEvent(InGameEvent inGameEvent, InGameEventSource source)
{
	m_InGameEventsNext[source].push_back(inGameEvent);
}

std::vector<GameEvent>* EventBus::GameEvents()
{
	return &m_GameEvents;
}

std::vector<InGameEvent>* EventBus::InGameEvents(InGameEventSource source)
{
	return &m_InGameEventsCurrent[source];
}

void EventBus::Proceed()
{
	m_GameEvents.clear();

	for (int i = 0; i < IN_GAME_EVENT_SOURCE_TOTAL; i++)
	{
		m_InGameEventsCurrent[i].clear();
	}

	auto *swap = m_InGameEventsCurrent;
	m_InGameEventsCurrent = m_InGameEventsNext;
	m_InGameEventsNext = swap;
}