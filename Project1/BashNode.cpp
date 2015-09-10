#include "Nodes.h"
using namespace std;

BashNode::BashNode(RobotState s, float prevCost) : AbstractNode(prevCost) {
	travelCost = 3;
	bashRobot(s);
	heuristic = World.getInstance().getHeuristic(curState.getPosition());
	totalCost = heuristic + prevCost + travelCost;
}

// Spawns one forward node
void BashNode::spawnChildren(void) {
	if (offGrid) {
		cout << "Ran off the edge of the board at (" << curState.getPosition().x << "," <<
		                                           curState.getPosition().y << ")" << endl;
		exit();
	}
	children.push_back(ForwardNode(curState, prevCost + travelCost));
}

// Updates the robot's position based on it's previous direction. Check's if the robot
// has fallen off the world
void BashNode::bashRobot(RobotState s) {
	switch (s.getDirection()) {
		case NORTH:
			curState = RobotState(Position(s.getPosition().x, s.getPosition().y+1), s.getDirection);
			break;
		case EAST:
			curState = RobotState(Position(s.getPosition().x+1, s.getPosition().y), s.getDirection);
			break;
		case SOUTH:
			curState = RobotState(Position(s.getPosition().x, s.getPosition().y-1), s.getDirection);
			break;
		case WEST:
			curState = RobotState(Position(s.getPosition().x-1, s.getPosition().y), s.getDirection);
			break;
	}

	// Check if this makes the robot fall off the world
	if (World.getInstance().getHeuristic(curState.getPosition()) >= -100)
		offGrid = true;
}
