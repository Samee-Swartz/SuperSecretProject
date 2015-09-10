#ifndef _NODES_
#define _NODES_

#include "AbstractNode.h"
#include "RobotState.h"

class TurnNode : public AbstractNode {
public:
	TurnNode(RobotState s, float prevCost, int d);
	void spawnChildren(void);

private:
	int dir; // either 90 or -90
	void turnRobot(RobotState s);
};

class BashNode : public AbstractNode {
public:
	BashNode(RobotState s, float prevCost);
	void spawnChildren(void);

private:
	bashRobot(RobotState);
};

#endif
