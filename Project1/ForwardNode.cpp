#include "AbstractNode.h"
#include "Nodes.h"
#include "World.h"
#include <stdio.h>

//ForwardNode constructor
ForwardNode::ForwardNode(RobotState rs, int p) : AbstractNode(rs, prevCost) {
	//keep direction of previous RobotState
	dir = rs.getRobotDirection();

	//update position dependent on direction
	Position thisPos = rs.getRobotPosition();
	switch (dir) {
		case NORTH:
			thisPos = new Position(rs.getRobotPosition().x, rs.getRobotPosition().y + 1);
			break;
		case EAST:
			thisPos = new Position(rs.getRobotPosition().x + 1, rs.getRobotPosition().y);
			break;
		case SOUTH:
			thisPos = new Position(rs.getRobotPosition().x, rs.getRobotPosition().y - 1);
			break;
		case WEST:
			thisPos = new Position(rs.getRobotPosition().x - 1, rs.getRobotPosition().y);
			break;
	}
	//assign thew new RobotState
	curState = RobotState(dir, thisPos);

	//Calculate the totalCost using prevCost + heuristic + travelCost
	prevCost = p;
	heuristic = World.getInstance().getHeuristic(curState.getPosition());
	travelCost = -1 - World.getInstance().getTerrain(curState.getPosition());
	totalCost = heuristic + prevCost + travelCost;
}

//A ForwardNode may spawn all 5 types of child nodes
void ForwardNode::spawnChildren(){
	//Turn left and right
	children.push_back(TurnNode(curState, prevCost + travelCost, 90));
	children.push_back(TurnNode(curState, prevCost + travelCost, -90));
	//Bash
	children.push_back(BashNode(curState, prevCost + travelCost));
	//Forward
	children.push_back(ForwardNode(curState, prevCost + travelCost));
	//Demolish
	children.push_back(DemolishNode(curState, prevCost + travelCost));

}
