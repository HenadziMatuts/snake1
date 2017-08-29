#include "snake.h"
#include "utilities.h"

bool Snake::IsAlive()
{
	return m_IsAlive;
}

void Snake::Spawn(int headPosX, int headPosY, int dirX, int dirY,
				int bodySize, int boundX, int boundY)
{
	m_HeadPosX = headPosX;
	m_HeadPosY = headPosY;
	m_BodySize = bodySize;
	if (dirX && dirY)
	{
		dirX = 0;
	}
	else if (!dirX && !dirY)
	{
		dirX = 1;
	}
	m_DirX = dirX;
	m_DirY = dirY;

	m_TailPosX = Utilities::ModuloSum(headPosX, -1 * dirX * bodySize, boundX);
	m_TailPosY = Utilities::ModuloSum(headPosY, -1 * dirY * bodySize, boundY);

	m_TailHops.clear();
	m_Locked = false;

	m_IsAlive = true;
	m_IsStoped = false;
	m_Growing = GROWING_FINISHED;
}

void Snake::Kill()
{
	m_IsAlive = false;
	m_TailHops.clear();
}

void Snake::Respawn(int headPosX, int headPosY, int dirX, int dirY,
				int bodySize, int boundX, int boundY)
{
	Kill();
	Spawn(headPosX, headPosY, dirX, dirY, bodySize, boundX, boundY);
}

void Snake::Grow(int boundX, int boundY)
{
	m_BodySize++;
	
	if (m_TailHops.empty())
	{
		m_TailPosX = Utilities::ModuloSum(m_TailPosX, (-1) * m_DirX, boundX);
		m_TailPosY = Utilities::ModuloSum(m_TailPosY, (-1) * m_DirY, boundY);
	}
	else
	{
		m_TailPosX = Utilities::ModuloSum(m_TailPosX, (-1) * TailDirX(), boundX);
		m_TailPosY = Utilities::ModuloSum(m_TailPosY, (-1) * TailDirY(), boundY);
		m_TailHops.front().m_hops++;
	}

	m_Growing = GROWING_STEP_1;
}

void Snake::Stop()
{
	m_IsStoped = true;

	//m_DirX = m_DirY = 0;
	//m_TailHops.clear();
}

void Snake::ChangeDirection(int newDirX, int newDirY)
{
	if ((m_DirX == newDirX * -1)
		|| (m_DirY == newDirY * -1))
	{
		return;
	}

	if (!m_Locked)
	{
		if (newDirX != m_DirX || newDirY != m_DirY)
		{
			int hops = m_BodySize;
			
			if (!m_TailHops.empty())
			{
				size_t size = m_TailHops.size();
				for (size_t i = 0; i < size; i++)
				{
					hops -= m_TailHops[i].m_hops;
				}	
			}
			if (hops > 0)
			{
				m_TailHops.push_back({ m_DirX, m_DirY, hops });
			}

			m_DirX = newDirX;
			m_DirY = newDirY;
			m_Locked = true;
		}
	}
}

void Snake::HandleInput(SDL_Event *event)
{
	if (event->type == SDL_KEYDOWN)
	{
		switch (event->key.keysym.sym)
		{
		case SDLK_UP:
			ChangeDirection(0, -1);
			break;
		case SDLK_DOWN:
			ChangeDirection(0, 1);
			break;
		case SDLK_RIGHT:
			ChangeDirection(1, 0);
			break;
		case SDLK_LEFT:
			ChangeDirection(-1, 0);
			break;
		default:
			break;
		}
	}
}

void Snake::Update(int boundX, int boundY)
{
	if (!IsStoped())
	{
		UpdateHeadPosition(boundX, boundY);
		UpdateTailPosition(boundX, boundY);

		m_Growing = (m_Growing == GROWING_FINISHED) ? m_Growing :
			(GrowingStatus)Utilities::ModuloSum(m_Growing, 1, GROWING_FINISHED + 1);
	}

	m_Locked = false;
}

int Snake::HeadPosX()
{
	return m_HeadPosX;
}

int Snake::HeadPosY()
{
	return m_HeadPosY;
}

int Snake::TailPosX()
{
	return m_TailPosX;
}

int Snake::TailPosY()
{
	return m_TailPosY;
}

int Snake::DirX()
{
	return m_DirX;
}

int Snake::DirY()
{
	return m_DirY;
}

int Snake::TailDirX()
{
	if (m_TailHops.empty())
	{
		return m_DirX;
	}

	return m_TailHops.front().m_dirX;
}

int Snake::TailDirY()
{
	if (m_TailHops.empty())
	{
		return m_DirY;
	}

	return m_TailHops.front().m_dirY;
}

int Snake::SegmentPosX(int n, int boundX)
{
	if (n <= 0)
	{
		return m_HeadPosX;
	}
	else if (n >= m_BodySize)
	{
		return m_TailPosX;
	}
	else if (m_TailHops.empty())
	{
		return Utilities::ModuloSum(m_HeadPosX, (-1) * n * m_DirX, boundX);
	}
	else
	{
		int segX = m_TailPosX;
		int x = 0, lasts = m_BodySize;

		for (int i = 0; i < (int)m_TailHops.size(); i++)
		{
			for (int j = 0; j < m_TailHops[i].m_hops; j++)
			{
				segX = Utilities::ModuloSum(segX, m_TailHops[i].m_dirX, boundX);
				if (++x == m_BodySize - n)
				{
					return segX;
				}
			}
			lasts -= m_TailHops[i].m_hops;
		}

		for (int i = 0; i < lasts; i++)
		{
			segX = Utilities::ModuloSum(segX, m_DirX, boundX);
			if (++x == m_BodySize - n)
			{
				return segX;
			}
		}
	}

	return -1;
}

int Snake::SegmentPosY(int n, int boundY)
{
	if (n <= 0)
	{
		return m_HeadPosY;
	}
	else if (n >= m_BodySize)
	{
		return m_TailPosY;
	}
	else if (m_TailHops.empty())
	{
		return Utilities::ModuloSum(m_HeadPosY, (-1) * n * m_DirY, boundY);
	}
	else
	{
		int segY = m_TailPosY;
		int y = 0, lasts = m_BodySize;

		for (int i = 0; i < (int)m_TailHops.size(); i++)
		{
			for (int j = 0; j < m_TailHops[i].m_hops; j++)
			{
				segY = Utilities::ModuloSum(segY, m_TailHops[i].m_dirY, boundY);
				if (++y == m_BodySize - n)
				{
					return segY;
				}
			}
			lasts -= m_TailHops[i].m_hops;
		}

		for (int i = 0; i < lasts; i++)
		{
			segY = Utilities::ModuloSum(segY, m_DirY, boundY);
			if (++y == m_BodySize - n)
			{
				return segY;
			}
		}
	}

	return -1;
}

int Snake::SegmentDirX(int n)
{
	if (n <= 0 || m_TailHops.empty())
	{
		return m_DirX;
	}
	else if (n >= m_BodySize)
	{
		return TailDirX();
	}
	else
	{
		int x = 0;

		for (int i = 0; i < (int)m_TailHops.size(); i++)
		{
			x += m_TailHops[i].m_hops;

			if (x > m_BodySize - n)
			{
				return m_TailHops[i].m_dirX;
			}
		}
	}

	return m_DirX;
}

int Snake::SegmentDirY(int n)
{
	if (n <= 0 || m_TailHops.empty())
	{
		return m_DirY;
	}
	else if (n >= m_BodySize)
	{
		return TailDirY();
	}
	else
	{
		int y = 0;

		for (int i = 0; i < (int)m_TailHops.size(); i++)
		{
			y += m_TailHops[i].m_hops;

			if (y > m_BodySize - n)
			{
				return m_TailHops[i].m_dirY;
			}
		}
	}

	return m_DirY;
}

int Snake::BodySize()
{
	return m_BodySize;
}

bool Snake::IsLocked()
{
	return m_Locked;
}

bool Snake::IsStoped()
{
	//return (m_DirX == 0 && m_DirY == 0) ? true : false;
	return m_IsStoped;
}

bool Snake::IsCrossedTheBound()
{
	return m_CrossedTheBound;
}

GrowingStatus Snake::Growing()
{
	return m_Growing;
}

void Snake::UpdateHeadPosition(int boundX, int boundY)
{
	m_CrossedTheBound = false;

	m_HeadPosX += m_DirX;
	m_HeadPosY += m_DirY;
	
	if (m_HeadPosX >= boundX || m_HeadPosX < 0
		|| m_HeadPosY >= boundY || m_HeadPosY < 0)
	{
		m_CrossedTheBound = true;
	}

	/* update head X position */
	m_HeadPosX = Utilities::ModuloSum(m_HeadPosX, 0, boundX);
	/* update head Y position */
	m_HeadPosY = Utilities::ModuloSum(m_HeadPosY, 0, boundY);
}

void Snake::UpdateTailPosition(int boundX, int boundY)
{
	/* update tail position */
	if (m_TailHops.empty())
	{
		/* no tail hops, moving tail in head direction */
		m_TailPosX = Utilities::ModuloSum(m_TailPosX, m_DirX, boundX);
		m_TailPosY = Utilities::ModuloSum(m_TailPosY, m_DirY, boundY);
	}
	else
	{
		m_TailPosX = Utilities::ModuloSum(m_TailPosX, m_TailHops.front().m_dirX, boundX);
		m_TailPosY = Utilities::ModuloSum(m_TailPosY, m_TailHops.front().m_dirY, boundY);

		if (--m_TailHops.front().m_hops == 0)
		{
			m_TailHops.pop_front();
		}
	}
}