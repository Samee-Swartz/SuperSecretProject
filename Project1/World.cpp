#include "World.h"
#include <stdlib.h>

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
w,h
*/

World::World(unsigned int in_width, unsigned int in_height)
{
	m_worldGrid = new int[in_width * in_height];
	m_width = in_width;
	m_height = in_height;
}

World& World::generateWorld() {
	int width = (rand() % 1000) + 10;
	int height = (rand() % 1000) + 10;
	m_instance = new World(width, height);

	for (int w = 0; w < m_width; w++) {
		for (int h = 0; h < m_height; h++) {
			m_instance[w][h] = (rand() % 9) + 1;
		}
	}
}

World& World::getInstance()
{
	return *m_instance;
}

int World::getTerrain(const Position& in_worldPosition) const
{
	if(!inWorld(in_worldPosition))
		return -100;

	return m_worldGrid[getArrayIndex(in_worldPosition)];
}

void World::setTerrain(const Position& in_worldPosition, int in_newValue)
{
	if(!inWorld(in_worldPosition))
		return;

	m_worldGrid[getArrayIndex(in_worldPosition)] = in_newValue;
}

bool World::isInWorld(const Position& in_worldPosition) const
{
	return in_worldPosition.x >= 0 && in_worldPosition.x < m_width && in_worldPosition.y >= 0 && in_worldPosition.y < m_width;
}

unsigned int World::getArrayIndex(const Position& in_worldPosition) const
{
	return (unsigned int)(in_worldPosition.x + in_worldPosition.y * m_height);
}
