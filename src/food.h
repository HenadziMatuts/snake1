#pragma once

class Food
{
public:
	Food() :
		m_IsAlive(false)
	{};

	bool IsAlive();

	int PosX();
	int PosY();

	void Spawn(int posX, int posY);
	void Kill();
	void Respawn(int posX, int posY);

	void Update();

private:
	int m_PosX, m_PosY;
	bool m_IsAlive;
};