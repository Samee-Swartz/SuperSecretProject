#include "AbstractNode.h"
#include "World.h"
#include <stdio.h>

class ForwardNode:AbstractNode {
public:

	ForwardNode::ForwardNode(RobotState rs, float p){
		//keep direction of previous RobotState
		 dir = rs.getRobotDirection();
		
		//update position dependent on direction
		Position thisPos = rs.getRobotPosition();
		if(dir == North){
			thisPos = new Position(rs.getRobotPosition().x, rs.getRobotPosition().y + 1);
		}
		else if(dir == East){
			thisPos = new Position(rs.getRobotPosition().x + 1, rs.getRobotPosition().y);
		}
		else if(dir == South){
			thisPos = new Position(rs.getRobotPosition().x, rs.getRobotPosition().y - 1);
		}
		else if(dir == West){
			thisPos = new Position(rs.getRobotPosition().x - 1, rs.getRobotPosition().y);
			
		}
		
		curState = new RobotState(thisDir, thisPos);
		prevCost = p;
		heuristic = World.getInstance().getHeuristic(curState.getPosition());
		travelCost = 1 + heuristic;
		totalCost = heuristic + prevCost + travelCost;
		
	}

	void ForwardNode::spawnChildren(){
		//all 5 (Tl, Tr, B, F, D)
		children.push_back(TurnNode(curState, prevCost + travelCost, 90));
		children.push_back(TurnNode(curState, prevCost + travelCost, -90));
		children.push_back(BashNode(curState, prevCost + travelCost));
		children.push_back(ForwardNode(curState, prevCost + travelCost));
		children.push_back(DemolishNode(curState,prevCost + travelCost));

	}

};
