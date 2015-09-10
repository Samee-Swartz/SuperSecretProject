#ifndef _ABSTRACTNODE_
#define _ABSTRACTNODE_

#include "RobotState.h"

// Abstract Class representing a Node.
class AbstractNode {
public:
	AbstractNode(int p) : prevCost(p) {
		offGrid = false;
		// Extended classes need to calculate heuristic, curSTate, travelCost, and totalCost
	}
	void onEnter(void) {};
	void onExit(void) {};
	virtual void spawnChildren(void) = 0;
	int getTotalCost() const { return totalCost; }

private:
	// determines whether this node moves the robot off the world
	bool offGrid;	// Calculated in constructor. If world terrain >= -100
	// heuristic to the goal
	int heuristic;
	// travel cost for this node on this terrain
	int travelCost;
	// previous cost to get to this nodefloat
	int prevCost;
	// total = heuristic + travel + previous
	int totalCost;
	// the state of the robot on this node
	RobotState curState;
};

#endif
