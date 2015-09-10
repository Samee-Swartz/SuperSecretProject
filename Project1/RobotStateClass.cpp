//Class declaration for the Robot's state
#include RobotState.h
#include ModData.h


RobotState::RobotState(Direction dir, struct Position pos){
	d = dir;
	p = pos;
}

Direction getRobotDirection(){
	return d;
}
	
struct Position getRobotPosition(){
	return p;
}