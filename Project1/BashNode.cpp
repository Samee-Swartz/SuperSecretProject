#include "Nodes.h"
#include "World.h"

using namespace std;

BashNode::BashNode(RobotState s, int prevCost, AbstractNode* parent)
: AbstractNode(s, prevCost, parent)
{
	travelCost = -3;
	bashRobot(s);
	heuristic = World::getInstance().calculateHeuristic(curState.getRobotPosition());
	totalCost = heuristic + prevCost + travelCost;
	offGrid = !World::getInstance().isInWorld(curState.getRobotPosition());
	m_isGoal = World::getInstance().isGoal(curState.getRobotPosition());
}

// Spawns one forward node
void BashNode::spawnChildren(void)
{
	children.push_back(new ForwardNode(curState, prevCost + travelCost, this));
}

// Updates the robot's position based on it's previous direction. Check's if the robot
// has fallen off the world
void BashNode::bashRobot(RobotState s)
{
	switch (s.getRobotDirection())
	{
		case NORTH:
			curState = RobotState(s.getRobotDirection(),
				Position(s.getRobotPosition().x, s.getRobotPosition().y + 1));
			break;
		case EAST:
			curState = RobotState(s.getRobotDirection(),
				Position(s.getRobotPosition().x + 1, s.getRobotPosition().y));
			break;
		case SOUTH:
			curState = RobotState(s.getRobotDirection(),
				Position(s.getRobotPosition().x, s.getRobotPosition().y - 1));
			break;
		case WEST:
			curState = RobotState(s.getRobotDirection(),
				Position(s.getRobotPosition().x - 1, s.getRobotPosition().y));
			break;
	}
}
