#include "game.h"
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

void GameField::Initilaize(pfnHandleInput HandleInput, pfnHandleCollisions handleCollisions, pfnRender render,
		bool stretch, uint32_t gridDimensionX, uint32_t gridDimensionY,
		int speed, int startBodySize, bool borderless)
{
	m_HandleInput = HandleInput;
	m_HandleCollisions = handleCollisions;
	m_Render = render;
	if (m_GridDimensionX != gridDimensionX
		|| m_GridDimensionY != gridDimensionY)
	{
		Resize(gridDimensionX, gridDimensionY, stretch, false);
	}
	m_GameSpeed = speed;
	m_StartBodySize = startBodySize;
	m_IsBorderless = borderless;
}

void GameField::Reconfigure(uint32_t gridDimensionX, uint32_t gridDimensionY,
		bool stretch, int speed, int startBodySize, bool borderless, bool justResize)
{
	if (m_GridDimensionX != gridDimensionX
		|| m_GridDimensionY != gridDimensionY
		|| justResize)
	{
		Resize(gridDimensionX, gridDimensionY, stretch, justResize);
	}
	m_GameSpeed = speed;
	m_StartBodySize = startBodySize;
	m_IsBorderless = borderless;
}

void GameField::Resize(uint32_t gridDimensionX, uint32_t gridDimensionY, bool stretch, bool justResize)
{
	if (!justResize)
	{
		if (m_Grid)
		{
			delete[] m_Grid;
			m_Grid = nullptr;
		}

		m_GridDimensionX = gridDimensionX;
		m_GridDimensionY = gridDimensionY;

		/* one dimensional array */
		m_Grid = new FieldCell[m_GridDimensionX * m_GridDimensionY];
		if (!m_Grid)
		{
			LOG_FATAL("%s: Memory allocation error!", __FUNCTION__);
			CRASH(ExitCallback);
		}
	}

	if (!stretch)
	{
		m_UpLeftCornOffsetY = 15;
		
		m_CellWitdh = m_CellHeight = (Globals::SCREEN_HEIGHT - (m_UpLeftCornOffsetY * 2)) / m_GridDimensionY;
		m_UpLeftCornOffsetY += ((Globals::SCREEN_HEIGHT - (m_UpLeftCornOffsetY * 2)) % m_GridDimensionY) / 2;

		int fieldW = Globals::SCREEN_HEIGHT - (m_UpLeftCornOffsetY * 2);

		while ((int)(m_CellWitdh * m_GridDimensionX) > fieldW)
		{
			--m_CellWitdh;
			--m_CellHeight;

			m_UpLeftCornOffsetY += m_GridDimensionY / 2;
		}

		if (Globals::ASPECT_RATIO == ASPECT_RATIO_16_9)
		{
			m_UpLeftCornOffsetX = (Globals::SCREEN_WIDTH / 2) -
				((m_CellWitdh * m_GridDimensionX) / 2);
		}
		else
		{
			m_UpLeftCornOffsetX = m_UpLeftCornOffsetY +
				((m_CellHeight * m_GridDimensionY) / 2) -
				((m_CellWitdh * m_GridDimensionX) / 2);
		}
	}
	else
	{
		m_UpLeftCornOffsetX = m_UpLeftCornOffsetY = 0;
		m_CellWitdh = (Globals::SCREEN_WIDTH / m_GridDimensionX) +
			((Globals::SCREEN_WIDTH % m_GridDimensionX) ? 1 : 0);

		m_CellHeight = Globals::SCREEN_HEIGHT / m_GridDimensionY +
			((Globals::SCREEN_HEIGHT % m_GridDimensionY) ? 1 : 0);
	}

	RecalculateField(justResize ? false : true);

	m_FieldRect.w = m_CellWitdh * m_GridDimensionX;
	m_FieldRect.h = m_CellHeight * m_GridDimensionY;
	m_FieldRect.x = m_Grid[0].m_Rect.x;
	m_FieldRect.y = m_Grid[0].m_Rect.y;
}

void GameField::HandleInput(SDL_Event *event)
{
	m_HandleInput(this, event);
}

void GameField::Update(uint32_t elapsed, EventBus *eventBus)
{
	m_Elapsed += elapsed;

	if (!m_Snake.IsAlive())
	{
		SpawnSnake();
		if (!m_Food.IsAlive())
		{
			SpawnFood();
		}
		return;
	}

	/* handle events from scoreboard */
	auto events = eventBus->InGameEvents(IN_GAME_EVENT_SOURCE_SCOREBOARD);
	for (size_t i = 0, n = events->size(); i < n; i++)
	{
		if ((*events)[i] == INGAME_EVENT_SNAKE_DIED)
		{
			m_ShouldStop = true;
		}
	}

	uint32_t upd = 1000 / m_GameSpeed;
	while (m_Elapsed >= upd)
	{
		if (!m_Food.IsAlive() && !m_Snake.IsStoped())
		{
			SpawnFood();
		}

		/* extinguish tail cell */
		m_Grid[(m_GridDimensionY *  m_Snake.TailPosX()) + m_Snake.TailPosY()].m_State = CELL_STATE_EMPTY;

		/* update snake position */
		m_Snake.Update(m_GridDimensionX, m_GridDimensionY);
			
		/* handle collisions */
		if (m_ShouldStop)
		{
			m_Snake.Stop();
		}
		else
		{
			Game::Instance().Events().PostInGameEvent(m_HandleCollisions(this), IN_GAME_EVENT_SOURCE_GAME_FIELD);
		}

		m_Elapsed -= upd;
	}
}

void GameField::Render(SDL_Renderer *renderer)
{
	m_Render(this, renderer);
}

int GameField::GetCellWidth()
{
	return m_CellWitdh;
}

int GameField::GeUpLeftCornOffsetX()
{
	return m_UpLeftCornOffsetX;
}

int GameField::GeUpLeftCornOffsetY()
{
	return m_UpLeftCornOffsetY;
}

uint32_t GameField::GetGridDimensionX()
{
	return m_GridDimensionX;
}

uint32_t GameField::GetGridDimensionY()
{
	return m_GridDimensionY;
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
	RecalculateField(true);

	m_ShouldStop = false;
}


void GameField::SpawnSnake()
{
	m_Snake.Spawn(Utilities::Random(5, m_GridDimensionX - 5),
		Utilities::Random(5, m_GridDimensionY - 5), Utilities::Random(-1, 1),
		Utilities::Random(-1, 1), m_StartBodySize, m_GridDimensionX, m_GridDimensionY);

	int headPosX = m_Snake.HeadPosX(), headPosY = m_Snake.HeadPosY();
	int tailPosX = m_Snake.TailPosX(), tailPosY = m_Snake.TailPosY();
	int dirX = m_Snake.DirX(), dirY = m_Snake.DirY();
	int bodySize = m_Snake.BodySize();

	m_Grid[(m_GridDimensionY * headPosX) + headPosY].m_State = CELL_STATE_SNAKE;

	if (dirX)
	{
		for (int i = 1; i < bodySize; i++)
		{
			unsigned int x = Utilities::ModuloSum(headPosX, -1 * i * dirX, m_GridDimensionX);
			m_Grid[(m_GridDimensionY * x) + headPosY].m_State = CELL_STATE_SNAKE;
		}
	}
	else if (dirY)
	{
		for (int i = 1; i < bodySize; i++)
		{
			unsigned int y = Utilities::ModuloSum(headPosY, -1 * i * dirY, m_GridDimensionY);
			m_Grid[(m_GridDimensionY * headPosX) + y].m_State = CELL_STATE_SNAKE;
		}
	}

	m_Grid[(m_GridDimensionY * tailPosX) + tailPosY].m_State = CELL_STATE_SNAKE;
}

void GameField::SpawnFood()
{
	m_Food.Spawn(Utilities::Random(0, m_GridDimensionX - 1),
		Utilities::Random(0, m_GridDimensionY - 1));

	while (m_Grid[(m_GridDimensionY * m_Food.PosX()) + m_Food.PosY()].m_State != CELL_STATE_EMPTY
		|| (m_Food.PosX() == m_Snake.TailPosX() && m_Food.PosY() == m_Snake.TailPosY()))
	{
		m_Food.Respawn(Utilities::Random(0, m_GridDimensionX - 1),
			Utilities::Random(0, m_GridDimensionY - 1));
	}

	m_Grid[(m_GridDimensionY * m_Food.PosX()) + m_Food.PosY()].m_State = CELL_STATE_FOOD;
}

void GameField::RecalculateField(bool clear)
{
	for (uint32_t i = 0; i < m_GridDimensionX; i++)
	{
		uint32_t row = m_GridDimensionY * i;
		for (uint32_t j = 0; j < m_GridDimensionY; j++)
		{
			m_Grid[row + j].m_Rect.w = m_CellWitdh;
			m_Grid[row + j].m_Rect.h = m_CellHeight;
			m_Grid[row + j].m_Rect.x = m_UpLeftCornOffsetX + (m_CellWitdh * i);
			m_Grid[row + j].m_Rect.y = m_UpLeftCornOffsetY + (m_CellHeight * j);
			if (clear)
			{
				m_Grid[row + j].m_State = CELL_STATE_EMPTY;
			}
		}
	}
}

void HandleInputInGame(GameField *_this, SDL_Event *event)
{
	/* pass input event to snake instance */
	_this->m_Snake.HandleInput(event);
}

InGameEvent HandleCollisionsInGame(GameField *_this)
{
	uint32_t headCellIndex = (_this->m_GridDimensionY * _this->m_Snake.HeadPosX()) + _this->m_Snake.HeadPosY();
	uint32_t tailCellIndex = (_this->m_GridDimensionY * _this->m_Snake.TailPosX()) + _this->m_Snake.TailPosY();

	if (!_this->m_IsBorderless && _this->m_Snake.IsCrossedTheBound())
	{
		if (!_this->m_Snake.IsStoped())
		{
			_this->m_Snake.Stop();
			return INGAME_EVENT_SNAKE_DIED;
		}
		else
		{
			return INGAME_EVENT_NOTHING_HAPPENS;
		}
	}

	switch (_this->m_Grid[headCellIndex].m_State)
	{
		case CELL_STATE_EMPTY:
			_this->m_Grid[headCellIndex].m_State = CELL_STATE_SNAKE;
			return INGAME_EVENT_NOTHING_HAPPENS;

		case CELL_STATE_SNAKE:
			if (!_this->m_Snake.IsStoped())
			{
				_this->m_Snake.Stop();
				return INGAME_EVENT_SNAKE_DIED;
			}
			else
			{
				return INGAME_EVENT_NOTHING_HAPPENS;
			}
				
		case CELL_STATE_FOOD:
			_this->m_Food.Kill();
			_this->m_Grid[headCellIndex].m_State = CELL_STATE_SNAKE;
			
			_this->m_Snake.Grow(_this->m_GridDimensionX, _this->m_GridDimensionY);
			if (!_this->m_IsBorderless && _this->m_Snake.IsCrossedTheBound())
			{
				if (!_this->m_Snake.IsStoped())
				{
					_this->m_Snake.Stop();
					return INGAME_EVENT_SNAKE_DIED;
				}
				else
				{
					return INGAME_EVENT_NOTHING_HAPPENS;
				}
			}
			
			return INGAME_EVENT_SNAKE_GROWN;

		default:
			return INGAME_EVENT_NOTHING_HAPPENS;
	}
}

void RenderInGame(GameField *_this, SDL_Renderer *renderer)
{
	uint32_t headCellIndex = (_this->m_GridDimensionY * _this->m_Snake.HeadPosX()) + _this->m_Snake.HeadPosY();
	uint32_t tailCellIndex = (_this->m_GridDimensionY * _this->m_Snake.TailPosX()) + _this->m_Snake.TailPosY();

	SDL_Color borderc = Globals::COLOR_SCHEME->m_Border;
	SDL_Color emptyc = Globals::COLOR_SCHEME->m_CellEmpty;
	SDL_Color snakec = Globals::COLOR_SCHEME->m_CellSnake;
	SDL_Color foodc = Globals::COLOR_SCHEME->m_CellFood;
	
	if (!_this->m_IsBorderless)
	{
		SDL_Rect r = _this->m_FieldRect;
		int offset = (_this->m_UpLeftCornOffsetX <= _this->m_UpLeftCornOffsetY) ?
			_this->m_UpLeftCornOffsetX : _this->m_UpLeftCornOffsetY;
		r.x -= offset / 2;
		r.y -= offset / 2;
		r.w += offset;
		r.h += offset;
		SDL_SetRenderDrawColor(renderer, borderc.r, borderc.g, borderc.b, borderc.a);
		SDL_RenderFillRect(renderer, &r);
	}

	SDL_SetRenderDrawColor(renderer, emptyc.r, emptyc.g, emptyc.b, emptyc.a);
	SDL_RenderFillRect(renderer, &_this->m_FieldRect);

	for (uint32_t i = 0; i < _this->m_GridDimensionX; i++)
	{
		uint32_t row = _this->m_GridDimensionY * i;
		for (uint32_t j = 0; j < _this->m_GridDimensionY; j++)
		{
			switch (_this->m_Grid[row + j].m_State)
			{
				case CELL_STATE_SNAKE:
					SDL_SetRenderDrawColor(renderer, snakec.r, snakec.g, snakec.b, snakec.a);
					SDL_RenderFillRect(renderer, &_this->m_Grid[row + j].m_Rect);
					break;
				case CELL_STATE_FOOD:
					SDL_SetRenderDrawColor(renderer, foodc.r, foodc.g, foodc.b, foodc.a);
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

		SDL_SetRenderDrawColor(renderer, emptyc.r, emptyc.g, emptyc.b, emptyc.a);
		SDL_RenderFillRect(renderer, &_this->m_Grid[headCellIndex].m_Rect);
		SDL_RenderFillRect(renderer, &_this->m_Grid[tailCellIndex].m_Rect);
		SDL_SetRenderDrawColor(renderer, snakec.r, snakec.g, snakec.b, snakec.a);
		
		/* head cell */
		SDL_Rect r = _this->m_Grid[headCellIndex].m_Rect;

		if (_this->m_Snake.Growing() == GROWING_STEP_1)
		{
			SDL_SetRenderDrawColor(renderer, foodc.r, foodc.g, foodc.b, foodc.a);
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
		SDL_SetRenderDrawColor(renderer, snakec.r, snakec.g, snakec.b, snakec.a);
		SDL_RenderFillRect(renderer, &r);

		/* tail */
		uint32_t cellIndex = tailCellIndex;
		
		if (_this->m_Snake.Growing() == GROWING_STEP_1)
		{
			int bodySize = _this->m_Snake.BodySize();
			int penultSegmentPosX = _this->m_Snake.SegmentPosX(bodySize - 1, _this->m_GridDimensionX);
			int penultSegmentPosY = _this->m_Snake.SegmentPosY(bodySize - 1, _this->m_GridDimensionY);
			uint32_t penultSegmentCellIndex = (_this->m_GridDimensionY * penultSegmentPosX) + penultSegmentPosY;

			SDL_SetRenderDrawColor(renderer, emptyc.r, emptyc.g, emptyc.b, emptyc.a);
			SDL_RenderFillRect(renderer, &_this->m_Grid[penultSegmentCellIndex].m_Rect);

			cellIndex = penultSegmentCellIndex;
		}

		r = _this->m_Grid[cellIndex].m_Rect;

		if (_this->m_Snake.Growing() == GROWING_STEP_2)
		{
			SDL_SetRenderDrawColor(renderer, emptyc.r, emptyc.g, emptyc.b, emptyc.a);
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
			SDL_SetRenderDrawColor(renderer, snakec.r, snakec.g, snakec.b, snakec.a);
			SDL_RenderFillRect(renderer, &r);
		}
	}

	if (_this->m_Snake.IsStoped())
	{
		if (_this->m_Snake.IsCrossedTheBound())
		{
			SDL_SetRenderDrawColor(renderer, emptyc.r, emptyc.g, emptyc.b, emptyc.a);
			SDL_RenderFillRect(renderer, &_this->m_Grid[headCellIndex].m_Rect);
		}

		if (Globals::SMOOTH_MOVEMENT)
		{
			if (_this->m_Snake.Growing() != GROWING_FINISHED)
			{
				SDL_SetRenderDrawColor(renderer, emptyc.r, emptyc.g, emptyc.b, emptyc.a);
			}
			else
			{
				SDL_SetRenderDrawColor(renderer, snakec.r, snakec.g, snakec.b, snakec.a);
			}
			SDL_RenderFillRect(renderer, &_this->m_Grid[tailCellIndex].m_Rect);
		}
		else
		{
			SDL_SetRenderDrawColor(renderer, snakec.r, snakec.g, snakec.b, snakec.a);
			SDL_RenderFillRect(renderer, &_this->m_Grid[tailCellIndex].m_Rect);
			
			if (_this->m_Snake.Growing() == GROWING_FINISHED)
			{
				tailCellIndex -= _this->m_GridDimensionX * _this->m_Snake.TailDirX();
				tailCellIndex -= _this->m_Snake.TailDirY();
				SDL_SetRenderDrawColor(renderer, snakec.r, snakec.g, snakec.b, snakec.a);
				SDL_RenderFillRect(renderer, &_this->m_Grid[tailCellIndex].m_Rect);
			}
		}
	}
}

void HandleInputDemo(GameField *_this, SDL_Event *event)
{
	return;
}

InGameEvent HandleCollisionsDemo(GameField *_this)
{
	uint32_t headCellIndex = (_this->m_GridDimensionY * _this->m_Snake.HeadPosX()) + _this->m_Snake.HeadPosY();

	switch (_this->m_Grid[headCellIndex].m_State)
	{
		case CELL_STATE_EMPTY:
			_this->m_Grid[headCellIndex].m_State = CELL_STATE_SNAKE;
			/* looking for a food */
			if (_this->m_Snake.DirX())
			{
				for (int i = 0; i < (int)_this->m_GridDimensionY; i++)
				{
					if (_this->m_Grid[(_this->m_GridDimensionY * _this->m_Snake.HeadPosX()) + i].m_State == CELL_STATE_FOOD)
					{
						_this->m_Snake.ChangeDirection(0, i < _this->m_Snake.HeadPosY() ? -1 : 1);
					}
				}
			}
			else if (_this->m_Snake.DirY())
			{
				for (int i = 0; i < (int)_this->m_GridDimensionX; i++)
				{
					if (_this->m_Grid[(i * _this->m_GridDimensionY) + _this->m_Snake.HeadPosY()].m_State == CELL_STATE_FOOD)
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
			
			_this->m_Snake.Grow(_this->m_GridDimensionX, _this->m_GridDimensionY);
			_this->m_Grid[(_this->m_GridDimensionY * _this->m_Snake.HeadPosX()) + _this->m_Snake.HeadPosY()].m_State = CELL_STATE_SNAKE;
			
			break;
	}

	return INGAME_EVENT_NOTHING_HAPPENS;
}

void RenderDemo(GameField *_this, SDL_Renderer *renderer)
{
	SDL_Color democ = Globals::COLOR_SCHEME->m_CellDemo;

	SDL_SetRenderDrawColor(renderer, democ.r, democ.g, democ.b, democ.a);
	for (uint32_t i = 0; i < _this->m_GridDimensionX; i++)
	{
		uint32_t row = _this->m_GridDimensionY * i;
		for (uint32_t j = 0; j < _this->m_GridDimensionY; j++)
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