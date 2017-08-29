#include "gameField.h"
#include "utilities.h"
#include "globals.h"
#include <cmath>

void ExitCallback();

GameField::~GameField()
{
	if (m_Grid)
	{
		delete[] m_Grid;
	}
}

void GameField::Initilaize(pfnHandleEvents handleEvents, pfnHandleCollisions handleCollisions,
					pfnRender render, uint32_t gridDimension, bool stretch, int speed, bool borderless)
{
	m_HandleEvents = handleEvents;
	m_HandleCollisions = handleCollisions;
	m_Render = render;
	if (m_GridDimension != gridDimension)
	{
		Resize(gridDimension, stretch);
	}
	m_GameSpeed = speed;
	m_IsBorderless = borderless;
}

void GameField::Reconfigure(uint32_t gridDimension, bool stretch, int speed, bool borderless)
{
	if (m_GridDimension != gridDimension)
	{
		Resize(gridDimension, stretch);
	}
	m_GameSpeed = speed;
	m_IsBorderless = borderless;
}

void GameField::Resize(uint32_t gridDimension, bool stretch)
{
	if (m_Grid)
	{
		delete[] m_Grid;
		m_Grid = nullptr;
	}

	m_GridDimension = gridDimension;

	/* one dimensional array */
	m_Grid = new FieldCell[m_GridDimension * m_GridDimension];
	if (!m_Grid)
	{
		LOG_FATAL("%s: Memory allocation error!", __FUNCTION__);
		CRASH(ExitCallback);
	}
	
	if (!stretch)
	{
		m_UpLeftCornOffset = 15;
		m_CellWitdh = m_CellHeight = (Globals::SCREEN_HEIGHT - (m_UpLeftCornOffset * 2)) / m_GridDimension;
		m_UpLeftCornOffset += ((Globals::SCREEN_HEIGHT - (m_UpLeftCornOffset * 2)) % m_GridDimension) / 2;
	}
	else
	{
		m_UpLeftCornOffset = 0;
		m_CellWitdh = Globals::SCREEN_WIDTH / m_GridDimension;
		m_CellWitdh += Globals::SCREEN_WIDTH % m_GridDimension ? 1 : 0;

		m_CellHeight = Globals::SCREEN_HEIGHT / m_GridDimension;
		m_CellHeight += Globals::SCREEN_HEIGHT % m_GridDimension ? 1 : 0;
	}

	ClearField();

	m_FieldRect.w = m_CellWitdh * m_GridDimension;
	m_FieldRect.h = m_CellHeight * m_GridDimension;
	m_FieldRect.x = m_Grid[0].m_Rect.x;
	m_FieldRect.y = m_Grid[0].m_Rect.y;
}

void GameField::HandleEvents(SDL_Event *event)
{
	m_HandleEvents(this, event);
}

InGameEvent GameField::Update(uint32_t elapsed)
{
	bool ret = false;
	InGameEvent e = INGAME_EVENT_NOTHING_HAPPENS;
	m_Elapsed += elapsed;

	if (!m_Snake.IsAlive())
	{
		SpawnSnake();
		if (!m_Food.IsAlive())
		{
			SpawnFood();
		}
		ret = true;
	}
	if (!ret)
	{
		uint32_t upd = 1000 / m_GameSpeed;
		while (m_Elapsed >= upd)
		{
			if (!m_Food.IsAlive() && !m_Snake.IsStoped())
			{
				SpawnFood();
			}

			/* extinguish tail cell */
			m_Grid[(m_GridDimension *  m_Snake.TailPosX()) + m_Snake.TailPosY()].m_State = CELL_STATE_EMPTY;

			/* update snake position */
			m_Snake.Update(m_GridDimension, m_GridDimension);
			
			/* handle collisions */
			e = m_HandleCollisions(this);

			m_Elapsed -= upd;
		}
	}

	return e;
}

void GameField::Render(SDL_Renderer *renderer)
{
	m_Render(this, renderer);
}

int GameField::GetCellWidth()
{
	return m_CellWitdh;
}

int GameField::GeUpLeftCornOffset()
{
	return m_UpLeftCornOffset;
}

uint32_t GameField::GetGridDimension()
{
	return m_GridDimension;
}

void GameField::Stop()
{
	m_Snake.Stop();
}

void GameField::Reset()
{
	if (m_Snake.IsAlive())
	{
		m_Snake.Kill();
	}
	if (m_Food.IsAlive())
	{
		m_Food.Kill();
	}
	ClearField();
}


void GameField::SpawnSnake()
{
	m_Snake.Spawn(Utilities::Random(5, m_GridDimension - 5),
		Utilities::Random(5, m_GridDimension - 5), Utilities::Random(-1, 1),
		Utilities::Random(-1, 1), 10, m_GridDimension, m_GridDimension);

	int headPosX = m_Snake.HeadPosX(), headPosY = m_Snake.HeadPosY();
	int tailPosX = m_Snake.TailPosX(), tailPosY = m_Snake.TailPosY();
	int dirX = m_Snake.DirX(), dirY = m_Snake.DirY();
	int bodySize = m_Snake.BodySize();

	m_Grid[(m_GridDimension * headPosX) + headPosY].m_State = CELL_STATE_SNAKE;

	if (dirX)
	{
		for (int i = 1; i < bodySize; i++)
		{
			unsigned int x = Utilities::ModuloSum(headPosX, -1 * i * dirX, m_GridDimension);
			m_Grid[(m_GridDimension * x) + headPosY].m_State = CELL_STATE_SNAKE;
		}
	}
	else if (dirY)
	{
		for (int i = 1; i < bodySize; i++)
		{
			unsigned int y = Utilities::ModuloSum(headPosY, -1 * i * dirY, m_GridDimension);
			m_Grid[(m_GridDimension * headPosX) + y].m_State = CELL_STATE_SNAKE;
		}
	}

	m_Grid[(m_GridDimension * tailPosX) + tailPosY].m_State = CELL_STATE_SNAKE;
}

void GameField::SpawnFood()
{
	m_Food.Spawn(Utilities::Random(0, m_GridDimension - 1),
		Utilities::Random(0, m_GridDimension - 1));

	while (m_Grid[(m_GridDimension * m_Food.PosX()) + m_Food.PosY()].m_State != CELL_STATE_EMPTY
		|| (m_Food.PosX() == m_Snake.TailPosX() && m_Food.PosY() == m_Snake.TailPosY()))
	{
		m_Food.Respawn(Utilities::Random(0, m_GridDimension - 1),
			Utilities::Random(0, m_GridDimension - 1));
	}

	m_Grid[(m_GridDimension * m_Food.PosX()) + m_Food.PosY()].m_State = CELL_STATE_FOOD;
}

void GameField::ClearField()
{
	for (uint32_t i = 0; i < m_GridDimension; i++)
	{
		uint32_t row = m_GridDimension * i;
		for (uint32_t j = 0; j < m_GridDimension; j++)
		{
			m_Grid[row + j].m_Rect.w = m_CellWitdh;
			m_Grid[row + j].m_Rect.h = m_CellHeight;
			m_Grid[row + j].m_Rect.x = m_UpLeftCornOffset + (m_CellWitdh * i);
			m_Grid[row + j].m_Rect.y = m_UpLeftCornOffset + (m_CellHeight * j);
			m_Grid[row + j].m_State = CELL_STATE_EMPTY;
		}
	}
}

void HandleEventsInGame(GameField *_this, SDL_Event *event)
{
	/* pass input event to snake instance */
	_this->m_Snake.HandleInput(event);
}

InGameEvent HandleCollisionsInGame(GameField *_this)
{
	uint32_t headCellIndex = (_this->m_GridDimension * _this->m_Snake.HeadPosX()) + _this->m_Snake.HeadPosY();
	uint32_t tailCellIndex = (_this->m_GridDimension * _this->m_Snake.TailPosX()) + _this->m_Snake.TailPosY();

	if (!_this->m_IsBorderless && _this->m_Snake.IsCrossedTheBound())
	{
		_this->m_Snake.Stop();
		return INGAME_EVENT_SNAKE_DIED;
	}

	switch (_this->m_Grid[headCellIndex].m_State)
	{
		case CELL_STATE_EMPTY:
			_this->m_Grid[headCellIndex].m_State = CELL_STATE_SNAKE;
			return INGAME_EVENT_NOTHING_HAPPENS;

		case CELL_STATE_SNAKE:
			_this->m_Snake.Stop();
			return INGAME_EVENT_SNAKE_DIED;

		case CELL_STATE_FOOD:
			_this->m_Food.Kill();
			_this->m_Grid[headCellIndex].m_State = CELL_STATE_SNAKE;
			
			_this->m_Snake.Grow(_this->m_GridDimension, _this->m_GridDimension);
			if (!_this->m_IsBorderless && _this->m_Snake.IsCrossedTheBound())
			{
				_this->m_Snake.Stop();
				return INGAME_EVENT_SNAKE_DIED;
			}
			
			return INGAME_EVENT_SNAKE_GROWN;

		default:
			return INGAME_EVENT_NOTHING_HAPPENS;
	}
}

void RenderInGame(GameField *_this, SDL_Renderer *renderer)
{
	uint32_t headCellIndex = (_this->m_GridDimension * _this->m_Snake.HeadPosX()) + _this->m_Snake.HeadPosY();
	uint32_t tailCellIndex = (_this->m_GridDimension * _this->m_Snake.TailPosX()) + _this->m_Snake.TailPosY();

	if (!_this->m_IsBorderless)
	{
		SDL_Rect r = _this->m_FieldRect;
		r.x -= _this->m_UpLeftCornOffset / 2;
		r.y -= _this->m_UpLeftCornOffset / 2;
		r.w += _this->m_UpLeftCornOffset;
		r.h += _this->m_UpLeftCornOffset;

		SDL_SetRenderDrawColor(renderer, 0xA0, 0xA0, 0xA0, 0xFF);
		SDL_RenderFillRect(renderer, &r);
	}

	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderFillRect(renderer, &_this->m_FieldRect);

	for (uint32_t i = 0; i < _this->m_GridDimension; i++)
	{
		uint32_t row = _this->m_GridDimension * i;
		for (uint32_t j = 0; j < _this->m_GridDimension; j++)
		{
			switch (_this->m_Grid[row + j].m_State)
			{
				case CELL_STATE_SNAKE:
					SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
					SDL_RenderFillRect(renderer, &_this->m_Grid[row + j].m_Rect);
					break;
				case CELL_STATE_FOOD:
					SDL_SetRenderDrawColor(renderer, 0xA0, 0xA0, 0xA0, 0xFF);
					SDL_RenderFillRect(renderer, &_this->m_Grid[row + j].m_Rect);
					break;
				default:
					break;
			}
		}
	}

	/* smooth movement */
	if (Globals::SMOOTH_MOVEMENT && !_this->m_Snake.IsStoped())
	{
		float f = (float)_this->m_Elapsed / (1000 / _this->m_GameSpeed);
		float dw, dh;

		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderFillRect(renderer, &_this->m_Grid[headCellIndex].m_Rect);
		SDL_RenderFillRect(renderer, &_this->m_Grid[tailCellIndex].m_Rect);
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

		/* head cell */
		SDL_Rect r = _this->m_Grid[headCellIndex].m_Rect;

		if (_this->m_Snake.Growing() == GROWING_STEP_1)
		{
			SDL_SetRenderDrawColor(renderer, 0xA0, 0xA0, 0xA0, 0xFF);
			SDL_RenderFillRect(renderer, &r);
		}

		dw = _this->m_CellHeight * (1 - f);
		dh = _this->m_CellWitdh * (1 - f);

		if (_this->m_Snake.DirX())
		{
			if (_this->m_Snake.IsLocked())
			{
				if (_this->m_Snake.SegmentDirY(1) < 0)
				{
					r.y += (int)(floorf(dh));
				}
				r.h -= (int)(floorf(dh));
			}
			else
			{
				if (_this->m_Snake.DirX() < 0)
				{
					r.x += (int)(floorf(dw));
				}
				r.w -= (int)(floorf(dw));
			}
		}
		else if (_this->m_Snake.DirY())
		{
			if (_this->m_Snake.IsLocked())
			{
				if (_this->m_Snake.SegmentDirX(1) < 0)
				{
					r.x += (int)(floorf(dw));
				}
				r.w -= (int)(floorf(dw));
			}
			else
			{
				if (_this->m_Snake.DirY() < 0)
				{
					r.y += (int)(floorf(dh));
				}
				r.h -= (int)(floorf(dh));
			}
		}
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderFillRect(renderer, &r);

		/* tail */
		uint32_t cellIndex = tailCellIndex;
		
		if (_this->m_Snake.Growing() == GROWING_STEP_1)
		{
			int bodySize = _this->m_Snake.BodySize();
			int penultSegmentPosX = _this->m_Snake.SegmentPosX(bodySize - 1, _this->m_GridDimension);
			int penultSegmentPosY = _this->m_Snake.SegmentPosY(bodySize - 1, _this->m_GridDimension);
			uint32_t penultSegmentCellIndex = (_this->m_GridDimension * penultSegmentPosX) + penultSegmentPosY;

			SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
			SDL_RenderFillRect(renderer, &_this->m_Grid[penultSegmentCellIndex].m_Rect);

			cellIndex = penultSegmentCellIndex;
		}

		r = _this->m_Grid[cellIndex].m_Rect;

		if (_this->m_Snake.Growing() == GROWING_STEP_2)
		{
			SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
			SDL_RenderFillRect(renderer, &r);
		}
		else
		{
			dw = _this->m_CellHeight * f;
			dh = _this->m_CellWitdh * f;

			if (_this->m_Snake.TailDirX())
			{
				if (_this->m_Snake.TailDirX() > 0)
				{
					r.x += (int)(floorf(dw));
				}
				r.w -= (int)(floorf(dw));
			}

			if (_this->m_Snake.TailDirY())
			{
				if (_this->m_Snake.TailDirY() > 0)
				{
					r.y += (int)(floorf(dh));
				}
				r.h -= (int)(floorf(dh));
			}
			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderFillRect(renderer, &r);
		}
	}

	if (_this->m_Snake.IsStoped())
	{
		if (_this->m_Snake.IsCrossedTheBound())
		{
			SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
			SDL_RenderFillRect(renderer, &_this->m_Grid[headCellIndex].m_Rect);
		}

		if (Globals::SMOOTH_MOVEMENT)
		{
			if (_this->m_Snake.Growing() != GROWING_FINISHED)
			{
				SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
			}
			else
			{
				SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			}
			SDL_RenderFillRect(renderer, &_this->m_Grid[tailCellIndex].m_Rect);
		}
		else
		{
			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderFillRect(renderer, &_this->m_Grid[tailCellIndex].m_Rect);
			
			if (_this->m_Snake.Growing() == GROWING_FINISHED)
			{
				tailCellIndex -= _this->m_GridDimension * _this->m_Snake.TailDirX();
				tailCellIndex -= _this->m_Snake.TailDirY();
				SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderFillRect(renderer, &_this->m_Grid[tailCellIndex].m_Rect);
			}
		}
	}
}

void HandleEventsDemo(GameField *_this, SDL_Event *event)
{
	return;
}

InGameEvent HandleCollisionsDemo(GameField *_this)
{
	uint32_t headCellIndex = (_this->m_GridDimension * _this->m_Snake.HeadPosX()) + _this->m_Snake.HeadPosY();

	switch (_this->m_Grid[headCellIndex].m_State)
	{
		case CELL_STATE_EMPTY:
			_this->m_Grid[headCellIndex].m_State = CELL_STATE_SNAKE;
			/* looking for a food */
			if (_this->m_Snake.DirX())
			{
				for (int i = 0; i < (int)_this->m_GridDimension; i++)
				{
					if (_this->m_Grid[(_this->m_GridDimension * _this->m_Snake.HeadPosX()) + i].m_State == CELL_STATE_FOOD)
					{
						_this->m_Snake.ChangeDirection(0, i < _this->m_Snake.HeadPosY() ? -1 : 1);
					}
				}
			}
			else if (_this->m_Snake.DirY())
			{
				for (int i = 0; i < (int)_this->m_GridDimension; i++)
				{
					if (_this->m_Grid[(i * _this->m_GridDimension) + _this->m_Snake.HeadPosY()].m_State == CELL_STATE_FOOD)
					{
						_this->m_Snake.ChangeDirection(i < _this->m_Snake.HeadPosX() ? -1 : 1, 0);
					}
				}
			}
			break;

		case CELL_STATE_SNAKE:
			_this->Reset();
			break;

		case CELL_STATE_FOOD:
			_this->m_Food.Kill();
			_this->m_Grid[headCellIndex].m_State = CELL_STATE_SNAKE;
			
			_this->m_Snake.Grow(_this->m_GridDimension, _this->m_GridDimension);
			_this->m_Grid[(_this->m_GridDimension * _this->m_Snake.HeadPosX()) + _this->m_Snake.HeadPosY()].m_State = CELL_STATE_SNAKE;
			
			break;
	}

	return INGAME_EVENT_NOTHING_HAPPENS;
}

void RenderDemo(GameField *_this, SDL_Renderer *renderer)
{
	SDL_SetRenderDrawColor(renderer, 0x80, 0x80, 0x80, 0xFF);
	for (uint32_t i = 0; i < _this->m_GridDimension; i++)
	{
		uint32_t row = _this->m_GridDimension * i;
		for (uint32_t j = 0; j < _this->m_GridDimension; j++)
		{
			switch (_this->m_Grid[row + j].m_State)
			{
				case CELL_STATE_FOOD:
				case CELL_STATE_SNAKE:
					SDL_RenderFillRect(renderer, &_this->m_Grid[row + j].m_Rect);
					break;
				default:
					break;
			}
		}
	}
}