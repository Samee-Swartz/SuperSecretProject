#include "Nodes.h"
using namespace std;

BashNode::BashNode(RobotState s, int prevCost) : AbstractNode(s, prevCost) {
	travelCost = -3;
	bashRobot(s);
	heuristic = World.getInstance().getHeuristic(curState.getRobotPosition());
	totalCost = heuristic + prevCost + travelCost;
}

// Spawns one forward node
void BashNode::spawnChildren(void) {
	children.push_back(ForwardNode(curState, prevCost + travelCost));
}

// Updates the robot's position based on it's previous direction. Check's if the robot
// has fallen off the world
void BashNode::bashRobot(RobotState s) {
	switch (s.getRobotDirection()) {
		case NORTH:
			curState = RobotState(Position(s.getRobotPosition().x, s.getRobotPosition().y+1),
			                      s.getRobotDirection());
			break;
		case EAST:
			curState = RobotState(Position(s.getRobotPosition().x+1, s.getRobotPosition().y),
			                      s.getRobotDirection());
			break;
		case SOUTH:
			curState = RobotState(Position(s.getRobotPosition().x, s.getRobotPosition().y-1),
			                      s.getRobotDirection());
			break;
		case WEST:
			curState = RobotState(Position(s.getRobotPosition().x-1, s.getRobotPosition().y),
			                      s.getRobotDirection());
			break;
	}
}
