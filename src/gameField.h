#pragma once
#include "snake.h"
#include "food.h"
#include <SDL.h>
#include <cstdint>

enum InGameEvent
{
	INGAME_EVENT_NOTHING_HAPPENS = 0,
	INGAME_EVENT_SNAKE_DIED,
	INGAME_EVENT_SNAKE_GROWN,
};

enum CellState
{
	CELL_STATE_EMPTY = 0,
	CELL_STATE_SNAKE,
	CELL_STATE_FOOD,
};

struct FieldCell {
	/* coordinates */
	SDL_Rect m_Rect;
	/* state -> color */
	CellState m_State;
};

class GameField;

typedef void (*pfnHandleEvents)(GameField *_this, SDL_Event *event);
typedef InGameEvent (*pfnHandleCollisions)(GameField *_this);
typedef void (*pfnRender)(GameField *_this, SDL_Renderer *renderer);

/**
 * Game field class.
 */
class GameField {
public:
	GameField() :
		m_Grid(nullptr),
		m_GridDimension(0),
		m_GameSpeed(30),
		m_Elapsed(0),
		m_IsBorderless(true)
	{};
	~GameField();

	void Initilaize(pfnHandleEvents handleEvents, pfnHandleCollisions handleCollisions,
				pfnRender render, uint32_t gridDimension, bool stretch, int speed, bool borderless);
	void Reconfigure(uint32_t gridDimension, bool stretch, int speed, bool borderless);
	void Resize(uint32_t gridDimension, bool stretch);

	void HandleEvents(SDL_Event *event);
	InGameEvent Update(uint32_t elapsed);
	void Render(SDL_Renderer *renderer);

	int GetCellWidth();
	int GeUpLeftCornOffset();
	uint32_t GetGridDimension();

	void Stop();
	void Reset();

	/**/
	friend void HandleEventsInGame(GameField *_this, SDL_Event *event);
	friend InGameEvent HandleCollisionsInGame(GameField *_this);
	friend void RenderInGame(GameField *_this, SDL_Renderer *renderer);

	friend void HandleEventsDemo(GameField *_this, SDL_Event *event);
	friend InGameEvent HandleCollisionsDemo(GameField *_this);
	friend void RenderDemo(GameField *_this, SDL_Renderer *renderer);

private:
	/* implementation */
	pfnHandleEvents m_HandleEvents;
	pfnHandleCollisions m_HandleCollisions;
	pfnRender m_Render;

	/* field grid properties */
	int m_UpLeftCornOffset;

	FieldCell *m_Grid;
	uint32_t m_GridDimension;

	SDL_Rect m_FieldRect;
	int m_CellWitdh, m_CellHeight;

	/* game speed */
	int m_GameSpeed;
	uint32_t m_Elapsed;

	bool m_IsBorderless;

	/* snake instance */
	Snake m_Snake;
	/* food instance */
	Food m_Food;

	void SpawnSnake();
	void SpawnFood();
	void ClearField();
};