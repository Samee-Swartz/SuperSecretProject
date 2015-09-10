#include "AbstractNode.h"
#include "World.h"
#include <stdio.h>

class SourceNode:AbstractNode {
public:

	SourceNode::SourceNode(RobotState rs){
		curState = rs;
		
		//Source Node has no total cost; all cost values start at 1. 
		prevCost = 0;
		heuristic = 0;
		travelCost = 0;
		totalCost = 0;
	}

	//The source node can spawn all types of Nodes
	void SourceNode::spawnChildren(){
		//both turns
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
