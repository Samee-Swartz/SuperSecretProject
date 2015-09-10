#include <AbstractNode.h>

class ForwardNode:AbstractNode {
public:

	ForwardNode(RobotState rs, float p){
		super(rs, p);
	}
	
	void ForwardNode::onEnter();
	void ForwardNode::onExit();
	
	void ForwardNode::spawnChildren(){
		//all 5 (Tl, Tr, B, F, D)
	
	}

}
