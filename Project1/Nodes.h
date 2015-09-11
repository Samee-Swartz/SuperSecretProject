#ifndef _NODES_
#define _NODES_

#include "World.h"
#include "AbstractNode.h"
#include "RobotState.h"

class TurnNode : public AbstractNode {
public:
	TurnNode(RobotState s, int prevCost, int d, AbstractNode* parent);
	void spawnChildren();

private:
	int dir; // either 90 or -90
	void turnRobot(RobotState s);
};

class BashNode : public AbstractNode {
public:
	BashNode(RobotState s, int prevCost, AbstractNode* parent);
	void spawnChildren();

private:
	void bashRobot(RobotState s);
};

class ForwardNode : public AbstractNode {
public:
	ForwardNode(RobotState s, int prevCost, AbstractNode* parent);
	void spawnChildren();
};

class SourceNode: public AbstractNode {
public:
	SourceNode(RobotState s);
	void spawnChildren();
};

class DemolishNode : public AbstractNode {
public:
	DemolishNode(RobotState s, int prevCost, AbstractNode* parent);
	void onEnter();
	void onExit();
	void spawnChildren();
};

#endif
