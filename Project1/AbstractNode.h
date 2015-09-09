
class AbstractNode {
public:
	AbstractNode(RobotState s, float p) : prevCost(p), curState(s) {
		heuristic = World.getInstance().getHeuristic();
		offGrid = false;
		// Extended classes need to calculate travelCost and totalCost
	}
	void onEnter(void) {};
	void onExit(void) {};
	virtual void spawnChildren(void) = 0;
	float getTotalCost() const { return totalCost; }

private:
	bool offGrid;	// Calculated in constructor. If world terrain >= -100
	float heuristic;
	float travelCost;
	float prevCost;
	float totalCost;
	RobotState curState;
};
