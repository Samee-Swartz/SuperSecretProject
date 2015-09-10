#include "AbstractNode.h"
#include "World.h"
#include <stdio.h>

class ForwardNode:AbstractNode {
public:

	//ForwardNode constructor 
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
		
		//assign thew new RobotState
		curState = new RobotState(thisDir, thisPos);
		
		//Calculate the totalCost using prevCost + heuristic + travelCost
		prevCost = p;
		heuristic = World.getInstance().getHeuristic(curState.getPosition());
		travelCost = 1 + heuristic;
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
		children.push_back(DemolishNode(curState,prevCost + travelCost));

	}

};
