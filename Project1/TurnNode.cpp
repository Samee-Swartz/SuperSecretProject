#include "RobotState.h"
#include "World.h"
#include <math.h>
#include <stdio.h>

using namespace std;

TurnNode::TurnNode(RobotState s, float prevCost, Direction d) : AbstractNode(prevCost) {
	dir = d;
	turnRobot(s);
	travelCost = -ceil((float)World.getInstance().getTerrain(curState.position) / 3.0);
	heuristic = World.getInstance().getHeuristic(curState.getPosition());
	totalCost = heuristic + prevCost + travelCost;
	// Extended classes need to calculate travelCost and totalCost
}

// Spawns Bash, Forward, Demolish, and Turn (in the same direction as this one)
void TurnNode::spawnChildren(void) {
	// does not including turning back because that's a waste of time
	children.push_back(TurnNode(curState, prevCost + travelCost, dir));  // turn in same direction
	children.push_back(BashNode(curState, prevCost + travelCost));
	children.push_back(ForwardNode(curState, prevCost + travelCost));
	children.push_back(DemolishNode(curState, prevCost + travelCost));
}

// Sets curState based on the given RobotState and this turn node.
void TurnNode::turnRobot(RobotState s) {
	switch (s.getDirection()) {
		case NORTH:
			if (dir == 90)
				curState = RobotState(s.getPosition(), EAST);
			else if (dir == -90)
				curState = RobotState(s.getPosition(), WEST);
			else
				cout << "dir was a bad value" << endl;
			break;
		case EAST:
			if (dir == 90)
				curState = RobotState(s.getPosition(), SOUTH);
			else if (dir == -90)
				curState = RobotState(s.getPosition(), NORTH);
			else
				cout << "dir was a bad value" << endl;
			break;
		case SOUTH:
			if (dir == 90)
				curState = RobotState(s.getPosition(), WEST);
			else if (dir == -90)
				curState = RobotState(s.getPosition(), EAST);
			else
				cout << "dir was a bad value" << endl;
			break;
		case WEST:
			if (dir == 90)
				curState = RobotState(s.getPosition(), NORTH);
			else if (dir == -90)
				curState = RobotState(s.getPosition(), SOUTH);
			else
				cout << "dir was a bad value" << endl;
			break;
	}
}
