
// Abstract Class representing a Node.
class AbstractNode {
public:
	AbstractNode(RobotState s, float p) : prevCost(p) {
		heuristic = World.getInstance().getHeuristic();
		offGrid = false;
		// Extended classes need to calculate curSTate, travelCost, and totalCost
	}
	void onEnter(void) {};
	void onExit(void) {};
	virtual void spawnChildren(void) = 0;
	float getTotalCost() const { return totalCost; }

private:
	// determines whether this node moves the robot off the world
	bool offGrid;	// Calculated in constructor. If world terrain >= -100
	// heuristic to the goal
	float heuristic;
	// travel cost for this node on this terrain
	float travelCost;
	// previous cost to get to this node
	float prevCost;
	// total = heuristic + travel + previous
	float totalCost;
	// the state of the robot on this node
	RobotState curState;
};
