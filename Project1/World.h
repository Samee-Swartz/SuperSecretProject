#ifndef _WORLD_
#define _WORLD_

#include <stack>
#include <string>
#include <stdlib.h>

#include "Position.h"
#include "ModData.h"
#include "RobotState.h"

class World
{
public:
	static World& generateWorld();
	static World& createWorldFrom(std::string);

	//Get the singleton instance of the world
	static World& getInstance();
public:
	//Get the terrain at a position
	int getTerrain(const Position& in_worldPosition) const;
	//Set the terrain at a position
	void setTerrain(const Position& in_worldPosition, int in_newValue);

	//Get the heuristic computed from the given Position
	int calculateHeuristic(const Position& in_pos);
	//Set the heuristic type
	void setHeuristic(int in_h) {heuristic = in_h;}

	//Tests if a point is in the world
	bool isInWorld(const Position& in_worldPosition) const;
	//Check if the Position is the goal
	bool isGoal(const Position& in_worldPosition) const;

	void pushMod(Mod m) {mods.push(m);}
	void popMod() {
		mods.top().restoreWorld();
		mods.pop();
	}

	RobotState getStartState() const {return m_startState;}

private:
	World(unsigned int in_width, unsigned int in_height);

	unsigned int getArrayIndex(const Position& in_worldPosition) const;
private:
	static World* m_instance;

	int* m_worldGrid;
	unsigned int m_width;
	unsigned int m_height;
	Position goal;
	Position start;
	// 1-6 given as program input
	int heuristic;
	std::stack<Mod> mods;

	RobotState m_startState;
};

#endif
