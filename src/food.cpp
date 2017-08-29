#include "food.h"
#include "utilities.h"

bool Food::IsAlive()
{
	return m_IsAlive;
}

int Food::PosX()
{
	return m_PosX;
}

int Food::PosY()
{
	return m_PosY;
}

void Food::Spawn(int posX, int posY)
{
	m_IsAlive = true;
	m_PosX = posX;
	m_PosY = posY;
}

void Food::Kill()
{
	m_IsAlive = false;
}

void Food::Respawn(int posX, int posY)
{
	Kill();
	Spawn(posX, posY);
}

void Food::Update()
{
}