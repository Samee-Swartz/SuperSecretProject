#include "AbstractNode.h"
#include "Nodes.h"
#include "World.h"
#include <stdio.h>

SourceNode::SourceNode(RobotState rs)
	: AbstractNode(rs, 0 , NULL)
{
	//Source Node has no total cost; all cost values start at 1. 
	prevCost = 0;
	heuristic = 0;
	travelCost = 0;
	totalCost = 0;
}

//The source node can spawn all types of Nodes
void SourceNode::spawnChildren()
{
	//both turns
	children.push_back(new TurnNode(curState, prevCost + travelCost, 90, this));
	children.push_back(new TurnNode(curState, prevCost + travelCost, -90, this));
	//Bash
	children.push_back(new BashNode(curState, prevCost + travelCost, this));
	//Forward
	children.push_back(new ForwardNode(curState, prevCost + travelCost, this));
	//Demolish
	children.push_back(new DemolishNode(curState,prevCost + travelCost, this));

}