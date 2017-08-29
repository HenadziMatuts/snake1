#pragma once
#include <SDL.h>
#include <deque>

enum GrowingStatus
{
	GROWING_STEP_1 = 0,
	GROWING_STEP_2,
	GROWING_FINISHED
};

struct TailHops
{
	int m_dirX;
	int m_dirY;
	int m_hops;
};

/**
 * Snake class.
 */
class Snake {
public:
	Snake() :
		m_IsAlive(false),
		m_CrossedTheBound(false),
		m_Growing(GROWING_FINISHED)
	{};

	bool IsAlive();
	int HeadPosX();
	int HeadPosY();
	int TailPosX();
	int TailPosY();
	int DirX();
	int DirY();
	int TailDirX();
	int TailDirY();
	int SegmentPosX(int n, int boundX);
	int SegmentPosY(int n, int boundY);
	int SegmentDirX(int n);
	int SegmentDirY(int n);
	int BodySize();
	bool IsLocked();
	bool IsStoped();
	bool IsCrossedTheBound();
	GrowingStatus Growing();

	void Spawn(int headPosX, int headPosY, int dirX, int dirY,
			int bodySize, int boundX, int boundY);
	void Kill();
	void Respawn(int headPosX, int headPosY, int dirX, int dirY,
			int bodySize, int boundX, int boundY);
	void Grow(int boundX, int boundY);
	void Stop();
		
	void HandleInput(SDL_Event *event);

	void Update(int boundX, int boundY);
	void ChangeDirection(int newDirX, int newDirY);

private:
	bool m_IsAlive;
	
	int m_HeadPosX, m_HeadPosY;
	int m_TailPosX, m_TailPosY;
	int m_DirX, m_DirY;
	int m_BodySize;
	
	/* stores tail hops after direction changes */
	std::deque<TailHops> m_TailHops;

	/* locks input until next update */
	bool m_Locked;
	bool m_CrossedTheBound;
	bool m_IsStoped;
	GrowingStatus m_Growing;

	void UpdateHeadPosition(int boundX, int boundY);
	void UpdateTailPosition(int boundX, int boundY);
};