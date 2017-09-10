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
		m_GridDimensionX(0),
		m_GridDimensionY(0),
		m_GameSpeed(30),
		m_Elapsed(0),
		m_StartBodySize(10),
		m_IsBorderless(true)
	{};
	~GameField();

	void Initilaize(pfnHandleEvents handleEvents, pfnHandleCollisions handleCollisions, pfnRender render,
			bool stretch, uint32_t gridDimensionX, uint32_t gridDimensionY,
			int speed, int startBodySize, bool borderless);
	void Reconfigure(uint32_t gridDimensionX, uint32_t gridDimensionY,
			bool stretch, int speed, int startBodySize, bool borderless, bool justResize);
	void Resize(uint32_t gridDimensionX, uint32_t gridDimensionY, bool stretch, bool justResize);

	void HandleEvents(SDL_Event *event);
	InGameEvent Update(uint32_t elapsed);
	void Render(SDL_Renderer *renderer);

	int GetCellWidth();
	int GeUpLeftCornOffsetX();
	int GeUpLeftCornOffsetY();
	uint32_t GetGridDimensionX();
	uint32_t GetGridDimensionY();

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

	int m_UpLeftCornOffsetX;
	int m_UpLeftCornOffsetY;

	FieldCell *m_Grid;
	
	uint32_t m_GridDimensionX;
	uint32_t m_GridDimensionY;

	SDL_Rect m_FieldRect;
	int m_CellWitdh, m_CellHeight;

	/* game speed */
	int m_GameSpeed;
	uint32_t m_Elapsed;

	int m_StartBodySize;

	bool m_IsBorderless;

	/* snake instance */
	Snake m_Snake;
	/* food instance */
	Food m_Food;

	void SpawnSnake();
	void SpawnFood();
	void RecalculateField(bool clear);
};