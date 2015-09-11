#ifndef _ROBOTSTATE_
#define _ROBOTSTATE_

#include "Position.h"

enum Direction { NORTH, SOUTH, EAST, WEST };

class RobotState{
	Direction d;
	Position p;
  public:
	RobotState(const Direction dir, const Position pos) : d(dir), p(pos) {}
	Direction& getRobotDirection() {return d;}
	Position& getRobotPosition() {return p;}
};

#endif
