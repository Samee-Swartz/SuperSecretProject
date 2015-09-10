#ifndef _ABSTRACTNODE_
#define _ABSTRACTNODE_

#include "RobotState.h"

// Abstract Class representing a Node.
class AbstractNode {
public:
	AbstractNode(int p, AbstractNode* parent): prevCost(p), offGrid(false)
	{}

	virtual void onEnter(void) {}
	virtual void onExit(void) {}

	virtual void spawnChildren(void) = 0;
	int getTotalCost() const { return totalCost; }

	vector<AbstractNode> getChildren() {return children;}
	bool isGoal() const { return isGoal; }
	bool isOffGrid() const { return offGrid; }
	bool isEnd() const { return isGoal || offGrid; }

	AbstractNode* getParent() const {return parent;}

protected:
	// determines whether this node moves the robot off the world
	bool offGrid;	// Calculated in constructor. If world terrain >= -100
	// heuristic to the goal
	int heuristic;
	// does this node reach a goal state
	bool isGoal;
	// travel cost for this node on this terrain
	int travelCost;
	// previous cost to get to this nodefloat
	int prevCost;
	// total = heuristic + travel + previous
	int totalCost;
	// the state of the robot on this node
	RobotState curState;
	// vector of children nodes. Created by spawnChildren
	vector<AbstractNode> children;

	AbstractNode* parent;
};

#endif
