#include "AbstractNode.h"
#include "Nodes.h"

DemolishNode::DemolishNode(RobotState s, int prevCost, AbstractNode* parent)
		: AbstractNode(s, prevCost, parent) 
{
	curState = s;
	travelCost = -4;
	heuristic = World::getInstance().calculateHeuristic(curState.getRobotPosition());
	totalCost = heuristic + prevCost + travelCost;
}

// creates a new mod to keep track of old world state.
void DemolishNode::onEnter() 
{
	World::getInstance().pushMod(Mod(curState.getRobotPosition()));
}

// restores old world state
void DemolishNode::onExit() 
{
	World::getInstance().popMod();
}

// Spawns Bash, Forward, Demolish, and Turn x2
void DemolishNode::spawnChildren(void) 
{
	children.push_back(new TurnNode(curState, prevCost + travelCost, 90, this));
	children.push_back(new TurnNode(curState, prevCost + travelCost, -90, this));
	children.push_back(new BashNode(curState, prevCost + travelCost, this));
	children.push_back(new ForwardNode(curState, prevCost + travelCost, this));
	children.push_back(new DemolishNode(curState, prevCost + travelCost, this));
}
