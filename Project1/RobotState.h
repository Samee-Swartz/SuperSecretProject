//Header file for the RobotStateClass
#include ModData.h

enum Direction { North, South, East, West };


class RobotState{
	direction d;
	stuct Position p;
  public:
	RobotState(direction, struct Position);
	getRobotDirection();
	getRobotPosition();
};