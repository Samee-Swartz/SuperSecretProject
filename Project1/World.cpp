#include "World.h"
#include "ModData.h"
#include <stdlib.h>
#include <time.h>

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

World::World(unsigned int in_width, unsigned int in_height) {
	m_worldGrid = new int[in_width * in_height];
	m_width = in_width;
	m_height = in_height;
}

World& World::generateWorld() {
	srand(time(NULL));
	// World size ranges from 10 - 1009
	m_width = (rand() % 1000) + 10;
	m_height = (rand() % 1000) + 10;
	m_instance = new World(width, height);

	for (int w = 0; w < m_width; w++) {
		for (int h = 0; h < m_height; h++) {
			m_instance[w][h] = '0'+(rand() % 9) + 1;
		}
	}
	goal = Position((rand() % (m_width-1))+1, (rand() % (m_height  -1))+1);
	start = goal;
	while (start == goal) {
		start = Position((rand() % (m_width-1))+1, (rand() % (m_height  -1))+1);
	}
	return *m_instance;
}

int World::calculateHeuristic(Position in_pos) {
	switch (heuristic) {
		case 1:
			return 0;
		case 2:
			return min(abs(in_pos.x - goal.x), abs(in_pos.y - goal.y));
		case 3:
			return max(abs(in_pos.x - goal.x), abs(in_pos.y - goal.y));
		case 4:
			return abs(in_pos.x - goal.x) + abs(in_pos.y - goal.y);
		case 5:
		case 6:
			int h = ceil(sqrt((float)(pow(in_pos.x - goal.x, 2) + pow(in_pos.y - goal.y, 2))))
			// 5. Find an admissable heuristic that dominates #4.  A small tweak of #4 will work here.
			if (heuristic == 5)
				return h;
			return h*3;
			// 6. Create a non-admissable heuristic by multiplying heuristic #5 by 3.
			//See the lecture notes on heuristics for why we might want to do such a thing.
		case default:
			cout << "invalid heuristic" << endl;
			return 0;
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

bool World::isGoal(const Position& in_worldPosition) const {
	return in_worldPosition.equals(goal);
}

unsigned int World::getArrayIndex(const Position& in_worldPosition) const
{
	return (unsigned int)(in_worldPosition.x + in_worldPosition.y * m_height);
}
