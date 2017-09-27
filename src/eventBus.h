#pragma once
#include <vector>

/**
* Game control flow events.
*/
enum GameEvent
{
	GAME_EVENT_NOTHING_HAPPENS = 0,
	GAME_EVENT_QUIT,
	GAME_EVENT_RESUME,
	GAME_EVENT_PAUSE,
	GAME_EVENT_START,
	GAME_EVENT_RESTART,
	GAME_EVENT_STOP
};

enum InGameEvent
{
	INGAME_EVENT_NOTHING_HAPPENS = 0,
	INGAME_EVENT_SNAKE_DIED,
	INGAME_EVENT_SNAKE_GROWN,
};

enum InGameEventSource
{
	IN_GAME_EVENT_SOURCE_GAME_FIELD = 0,
	IN_GAME_EVENT_SOURCE_SCOREBOARD,
	IN_GAME_EVENT_SOURCE_TOTAL
};

class EventBus {
public:
	EventBus() :
		m_InGameEventsCurrent(m_InGameEventsA),
		m_InGameEventsNext(m_InGameEventsB)
	{
		m_GameEvents.reserve(32);

		for (int i = 0; i < IN_GAME_EVENT_SOURCE_TOTAL; i++)
		{
			m_InGameEventsA[i].reserve(32);
			m_InGameEventsB[i].reserve(32);
		}
	}

	void PostGameEvent(GameEvent gameEvent);
	void PostInGameEvent(InGameEvent inGameEvent, InGameEventSource source);

	std::vector<GameEvent>* GameEvents();
	std::vector<InGameEvent>* InGameEvents(InGameEventSource source);

	void Proceed();

private:
	std::vector<GameEvent> m_GameEvents;

	std::vector<InGameEvent> m_InGameEventsA[IN_GAME_EVENT_SOURCE_TOTAL];
	std::vector<InGameEvent> m_InGameEventsB[IN_GAME_EVENT_SOURCE_TOTAL];

	std::vector<InGameEvent> *m_InGameEventsCurrent;
	std::vector<InGameEvent> *m_InGameEventsNext;
};