#include "World.h"

/*

y	^
	|
	|
	|
	|
	|
	|
	 ------------------>
0,0						x

*/





World::World(unsigned int in_width, unsigned int in_height)
{
	m_worldGrid = new int[in_width * in_height];
	m_width = in_width;
	m_height = in_height;
}

static World& create(unsigned int width, unsigned int height)
{
	if(!m_instance)
	{
		m_instance = new World(width, height);
	}

	return *m_instance;
}

static World& getInstance()
{
	return *m_instance;
}

int getTerrain(const Position& in_worldPosition) const
{
	if(!inWorld(in_worldPosition))
		return -100;

	return m_worldGrid[getArrayIndex(in_worldPosition)];
}

void setTerrain(const Position& in_worldPosition, int in_newValue)
{
	if(!inWorld(in_worldPosition))
		return;

	m_worldGrid[getArrayIndex(in_worldPosition)] = in_newValue;
}

bool isInWorld(const Position& in_worldPosition) const
{
	return in_worldPosition.x >= 0 && in_worldPosition.x < m_width && in_worldPosition.y >= 0 && in_worldPosition.y < m_width;
}

unsigned int getArrayIndex(const Position& in_worldPosition) const
{
	return (unsigned int)(in_worldPosition.x + in_worldPosition.y * m_height);
}
