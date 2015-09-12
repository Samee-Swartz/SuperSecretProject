#include "AbstractNode.h"
#include "Nodes.h"
#include "World.h"
#include <stdio.h>

//ForwardNode constructor
ForwardNode::ForwardNode(RobotState rs, int p, AbstractNode* parent) : AbstractNode(rs, prevCost, parent) {
	//keep direction of previous RobotState
	Direction dir = rs.getRobotDirection();

	//update position dependent on direction
	Position thisPos = rs.getRobotPosition();
	switch (dir) {
		case NORTH:
			thisPos = Position(rs.getRobotPosition().x, rs.getRobotPosition().y + 1);
			break;
		case EAST:
			thisPos = Position(rs.getRobotPosition().x + 1, rs.getRobotPosition().y);
			break;
		case SOUTH:
			thisPos = Position(rs.getRobotPosition().x, rs.getRobotPosition().y - 1);
			break;
		case WEST:
			thisPos = Position(rs.getRobotPosition().x - 1, rs.getRobotPosition().y);
			break;
	}
	//assign thew new RobotState
	curState = RobotState(dir, thisPos);

	//Calculate the totalCost using prevCost + heuristic + travelCost
	prevCost = p;
	heuristic = World::getInstance().calculateHeuristic(curState.getRobotPosition());
	travelCost = World::getInstance().getTerrain(curState.getRobotPosition());
	totalCost = heuristic + prevCost + travelCost;
	offGrid = !World::getInstance().isInWorld(curState.getRobotPosition());
	m_isGoal = World::getInstance().isGoal(curState.getRobotPosition());
}

//A ForwardNode may spawn all 5 types of child nodes
void ForwardNode::spawnChildren(){
	//Turn left and right
	children.push_back(new TurnNode(curState, prevCost + travelCost, 90, this));
	children.push_back(new TurnNode(curState, prevCost + travelCost, -90, this));
	//Bash
	children.push_back(new BashNode(curState, prevCost + travelCost, this));
	//Forward
	children.push_back(new ForwardNode(curState, prevCost + travelCost, this));
	//Demolish
	children.push_back(new DemolishNode(curState, prevCost + travelCost, this));

}
