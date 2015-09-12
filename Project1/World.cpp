#include "World.h"
#include "ModData.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstddef>
#include <vector>
#include <algorithm>

using namespace std;

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
r,c
*/

World* World::m_instance = NULL;

World::World(unsigned int in_width, unsigned int in_height)
	: m_startState(NORTH, Position(0, 0)),
	start(0, 0),
	goal(0, 0)
{
	m_worldGrid = new char[in_width * in_height];
	m_width = in_width;
	m_height = in_height;
	expandedNodes = 0;
}

World& World::generateWorld() {
	srand(time(NULL));
	// World size ranges from 10 - 1009
	unsigned int width = (rand() % 1000) + 10;
	unsigned int height = (rand() % 1000) + 10;
	m_instance = new World(width, height);

	for (int w = 0; w < width; w++)
	{
		for (int h = 0; h < height; h++)
		{
			m_instance->setTerrain(Position(w, h), (rand() % 9) + 1);
		}
	}

	m_instance->goal = Position((rand() % (width-1))+1, (rand() % (height  -1))+1);
	m_instance->start = m_instance->goal;
	while (m_instance->start == m_instance->goal) {
		m_instance->start = Position((rand() % (width-1))+1, (rand() % (height  -1))+1);
	}

	m_instance->m_startState = RobotState(NORTH, m_instance->start);

	return *m_instance;
}

World& World::createWorldFrom(std::string file) {
	std::vector<std::vector<char> > tempWorld;
	std::ifstream givenWorld(file.c_str());
	Position s, g;
	std::string line;
	int rows = 0, cols = 0;
	while (std::getline(givenWorld, line)) {
		std::vector<char> tempRow;
		std::istringstream iss(line);
		char c;
		while ( iss >> c) {
			tempRow.push_back(c);
			if (c == 'S') {
				s.x = rows;
				s.y = cols;
			}
			if (c == 'G') {
				g.x = rows;
				g.y = cols;
			}
			cols++;
		}
		tempWorld.insert(tempWorld.begin(), tempRow);
		rows++;
	}
	cols /= rows;

	m_instance = new World(rows, cols);

	for (int w = 0; w < rows; w++)
	{
		for (int h = 0; h < cols; h++)
		{
			m_instance->setTerrain(Position(w, h), tempWorld[w][h]);
			std::cout << tempWorld[w][h] << "  ";
		}
		std::cout << std::endl;
	}

	m_instance->start = Position(s.x, s.y - (s.x*cols));
	m_instance->goal = Position(g.x, g.y - (g.x*cols));
	m_instance->m_startState = RobotState(NORTH, m_instance->start);
	return *m_instance;
}

int World::calculateHeuristic(const Position& in_pos) {
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
		{
			int h = ceil(sqrt((float)(pow(in_pos.x - goal.x, 2) + pow(in_pos.y - goal.y, 2))));
			// 5. Find an admissable heuristic that dominates #4.  A small tweak of #4 will work here.
			if (heuristic == 5)
				return h;
			return h*3;
		}
			// 6. Create a non-admissable heuristic by multiplying heuristic #5 by 3.
			//See the lecture notes on heuristics for why we might want to do such a thing.
		default:
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
	if(!isInWorld(in_worldPosition))
		return 100;

	return (int)'0' - (int)m_worldGrid[getArrayIndex(in_worldPosition)];
}

void World::setTerrain(const Position& in_worldPosition, int in_newValue)
{
	if(!isInWorld(in_worldPosition))
		return;

	m_worldGrid[getArrayIndex(in_worldPosition)] = (char)((int)'0' - in_newValue);
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
	return (unsigned int)(in_worldPosition.x + in_worldPosition.y * m_width);
}
