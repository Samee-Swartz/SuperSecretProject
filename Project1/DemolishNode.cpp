#include "AbstractNode.h"
#include "Nodes.h"

DemolishNode::DemolishNode(RobotState s, int prevCost)
		: AbstractNode(s, prevCost) {
	curState = s;
	travelCost = -4;
	heuristic = World.getInstance().getHeuristic(curState.getPosition());
	totalCost = heuristic + prevCost + travelCost;
}

// creates a new mod to keep track of old world state.
void DemolishNode::onEnter() {
	World.getInstance().pushMod(Mod(curState.getRobotPosition()));
}

// restores old world state
void DemolishNode::onExit() {
	World.getInstance().popMod();
}

// Spawns Bash, Forward, Demolish, and Turn x2
void DemolishNode::spawnChildren(void) {
	children.push_back(TurnNode(curState, prevCost + travelCost, 90));
	children.push_back(TurnNode(curState, prevCost + travelCost, -90));
	children.push_back(BashNode(curState, prevCost + travelCost));
	children.push_back(ForwardNode(curState, prevCost + travelCost));
	children.push_back(DemolishNode(curState, prevCost + travelCost));
}
