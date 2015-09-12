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
#include <sstream>

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
	m_worldGrid = new int[in_width * in_height];
	m_width = in_width;
	m_height = in_height;
}

void World::saveWorld() {
	std::stringstream ss;
	ss << "generatedWorld-" << m_width << "x" << m_height << ".txt";
	std::ofstream file;
	file.open(ss.str().c_str());
	for (int w = 0; w < m_width; w++) {
		for (int h = 0; h < m_height; h++) {
			if (m_instance->start.x == w && (m_height - m_instance->start.y) == h)
				file << 'S';
			else if (m_instance->goal.x == w && (m_height - m_instance->goal.y) == h)
				file << 'G';
			else
				file << m_instance->getTerrain(Position(w,h));
			file << "\t";
		}
		file << "\n";
	}
	file.close();
	std::cout << "file saved to " << ss.str() << std::endl;
}

// generates a world with varying difficulty. difficulty should be between 1 and 5
World& World::generateWorld(int difficulty) {
	srand(time(NULL));
	// World size ranges from 10 - 1000

	unsigned int width;
	unsigned int height;

	switch(difficulty) {
		case 1: // board size 5 - 10
			width = (rand() % 5) + 5;
			height = (rand() % 5) + 5;
			break;
		case 2: // board size 10 - 100
			width = (rand() % 500) + 50;
			height = (rand() % 500) + 50;
			break;
		case 3: // board size 100 - 300
			width = (rand() % 200) + 100;
			height = (rand() % 200) + 100;
			break;
		case 4: // board size 300 - 800
			width = (rand() % 500) + 3000;
			height = (rand() % 500) + 3000;
			break;
		case 5: // board size 800 - 1000
			width = (rand() % 200) + 800;
			height = (rand() % 200) + 800;
			break;
		default:
			std::cout << "Bad difficulty value. for your stupidity you get difficulty 1." << std::endl;
			width = (rand() % 1000) + 10;
			height = (rand() % 1000) + 10;
			break;
	}

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
	m_instance->saveWorld();

	return *m_instance;
}

World& World::createWorldFrom(std::string file) {
	std::vector<std::vector<char> > tempWorld;
	std::ifstream givenWorld(file.c_str());
	if (!givenWorld) {
		std::cout << "Given input file is invalid, randomly generating a world" << std::endl;
		return generateWorld(3);
	}
	std::string line;
	int rows = 0, cols = 0;
	while (std::getline(givenWorld, line)) {
		std::vector<char> tempRow;
		std::istringstream iss(line);
		char c;
		while ( iss >> c) {
			tempRow.push_back(c);
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
			char worldValue = tempWorld[w][h];

			if (worldValue == 'G' || worldValue == 'S')
			{
				m_instance->setTerrain(Position(h, w), 1);
				if (worldValue == 'G')
					m_instance->goal = Position(h, w);
				else
					m_instance->start = Position(h, w);
			}
			else
				m_instance->setTerrain(Position(h, w), worldValue - '0');

		}
	}

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

	return m_worldGrid[getArrayIndex(in_worldPosition)];
}

void World::setTerrain(const Position& in_worldPosition, int in_newValue)
{
	if(!isInWorld(in_worldPosition))
		return;

	m_worldGrid[getArrayIndex(in_worldPosition)] = in_newValue;
}

bool World::isInWorld(const Position& in_worldPosition) const
{
	return in_worldPosition.x >= 0 && in_worldPosition.x < m_width && in_worldPosition.y >= 0 && in_worldPosition.y < m_height;
}

bool World::isGoal(const Position& in_worldPosition) const {
	return in_worldPosition.equals(goal);
}

unsigned int World::getArrayIndex(const Position& in_worldPosition) const
{
	return (unsigned int)(in_worldPosition.x + in_worldPosition.y * m_width);
}
