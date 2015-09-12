#include "Nodes.h"

#include "RobotState.h"
#include "World.h"
#include <math.h>
#include <iostream>
#include <stdio.h>

using namespace std;

TurnNode::TurnNode(RobotState s, int prevCost, int d, AbstractNode* parent) : AbstractNode(s, prevCost, parent) {
	dir = d;
	turnRobot(s);
	travelCost = ceil((float)World::getInstance().getTerrain(curState.getRobotPosition()) / 3.0);
	heuristic = World::getInstance().calculateHeuristic(curState.getRobotPosition());
	totalCost = heuristic + prevCost + travelCost;
	offGrid = !World::getInstance().isInWorld(curState.getRobotPosition());
	m_isGoal = World::getInstance().isGoal(curState.getRobotPosition());
}

// Spawns Bash, Forward, Demolish, and Turn (in the same direction as this one)
void TurnNode::spawnChildren(void) {
	// does not including turning back because that's a waste of time
	children.push_back(new TurnNode(curState, prevCost + travelCost, dir, this));  // turn in same direction
	children.push_back(new BashNode(curState, prevCost + travelCost, this));
	children.push_back(new ForwardNode(curState, prevCost + travelCost, this));
	children.push_back(new DemolishNode(curState, prevCost + travelCost, this));
}

// Sets curState based on the given RobotState and this turn node.
void TurnNode::turnRobot(RobotState s) {
	switch (s.getRobotDirection()) {
		case NORTH:
			if (dir == 90)
				curState = RobotState(EAST, s.getRobotPosition());
			else if (dir == -90)
				curState = RobotState(WEST, s.getRobotPosition());
			else
				cout << "dir was a bad value" << endl;
			break;
		case EAST:
			if (dir == 90)
				curState = RobotState(SOUTH, s.getRobotPosition());
			else if (dir == -90)
				curState = RobotState(NORTH, s.getRobotPosition());
			else
				cout << "dir was a bad value" << endl;
			break;
		case SOUTH:
			if (dir == 90)
				curState = RobotState(WEST, s.getRobotPosition());
			else if (dir == -90)
				curState = RobotState(EAST, s.getRobotPosition());
			else
				cout << "dir was a bad value" << endl;
			break;
		case WEST:
			if (dir == 90)
				curState = RobotState(NORTH, s.getRobotPosition());
			else if (dir == -90)
				curState = RobotState(SOUTH, s.getRobotPosition());
			else
				cout << "dir was a bad value" << endl;
			break;
	}
}
