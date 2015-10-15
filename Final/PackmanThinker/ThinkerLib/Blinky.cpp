#include "Blinky.h"
#include "Pawn.h"
#include "World.h"

	// Keeps track of node information during A* computation.
struct ScoredNode {
        PathNode* node; // current node
  	
	    // cost to get to this node (all previous travel + connectionCost) + curNodeValue if applicable
        float arrivalAndNodeCost;
        float totalScore; // arrivalAndNodeCost + heuristic
        // the first direction taken (leaving pacman's current position) to get to this node
        Direction::Enum origDirection;

        ScoredNode(PathNode* m_node, float m_aanc, float m_heuristic, Direction::Enum m_dir) {
                node = m_node;
                origDirection = m_dir;
                arrivalAndNodeCost = m_aanc;
                totalScore = m_aanc + m_heuristic;
        }

        ScoredNode() {}

};

Direction OnBlinkyThink(const Pawn& in_ourPawn, const World& in_ourWorld, float in_deltaTime, float in_totalTime){

	std::vector<PathNode*> world;
	in_ourWorld.GetNodes(world);
	if(in_ourWorld.getPacmanPowercell())
		state = 3;


	static int state;

	switch(state){

		case 1:
			int startingTargetNode = in_ourWorld.getNode(nodeID);
			//starting mode: Scatter
			if(in_totalTime <= 30) //at start of game
				return aStar(in_ourPawn.getClosestNode(), startingTargetNode);	//start to navigate to respective corner 
			//time this so that it won't actually reach corner by the time limit
			
			else
				state = 1;

			break;

		case 2:
			//Chase
			//standard mode, attempt to move to the node that PacMan is currently occupying
			//Use A* for this
			return aStar(in_ourPawn.getClosestNode(), in_ourWorld.getPacman().getClosestNode());
			
			
			break;

		case 3:
                        //Frightened
                        //Pacman has eaten a power cell thingy
                        //Attempt to navigate to same corner as scatter
                        int frightTargetNode = in_ourWorld.getNode(frightNodeID);
                        return aStar(in_ourPawn.getClosestNode(), startingTargetNode);  //start to navigate to respective corner

			break;
	}
}

//Calculates the straight line distance between the currently node in examination and the target node
float crowDist(aNode, bNode){
        float dist;
        dist = sqrt((bNode.getPosition().x - aNode.getPosition().x)^2 +(bNode.getPosition().y - aNode.getPosition().y)^2));
        return dist;

}


//pushes all nodes from queue A into queue B
void pushAll(std::Queue<ScoredNode> queueA, std::Queue<ScoredNode> queueB){
	for(j = 0; j <= queueA.size(); i++){
		ScoredNode aTemp = queueA.pop();
		queueB.push(aTemp);
	}


}

// expands the current node in all four directions. Adds valid expansions to the frontier list
void Expand(const World& in_world, ScoredNode in_curNode, std::priority_queue<ScoredNode>& out_frontier) {

	for (int i=0; i < 4; i++) {	// check all four directions
		PathNodeConnection con = in_curNode.node->GetConnection((Direction::Enum)i);	// connection in that direction
		if (con.IsValid()) {	// valid connection
			ScoredNode newNode;
			PathNode* p = in_world.GetNode(con.GetOtherNodeId());	// get next node
			// add connection cost to the cost to get to to current Node
			float score = con.GetCost() + in_curNode.arrivalAndNodeCost;
			newNode = ScoredNode(p, score, heuristic(p), in_curNode.origDirection); // keep orig direction

			out_frontier.push(newNode);	// add the valid new node to the frontier
		}
	}
}

//run A* from the closest node to Blinky to the closest node that Pacman is occupying
Direction aStar(currentNode, targetNode){
	std::Queue<ScoredNode> aStarQueue;
	std::Queue<ScoredNode> tempQueue;
	
	Expand(in_ourWorld, currentNode, aStarQueue);
	
	while(aStarQueue.front() != targetNode){

		Expand(in_ourWorld, aStarQueue.front(), aStarQueue);
		
		//goes through the current queue of nodes in the list and places the new node in the proper place
		for(int i = 0; i <= aStarQueue.size(); i++){
			aNode = aStarQueue.pop();
			if(aNode.totalValue > tempNode.totalValue){
				aStarQueue.push(tempNode);
				pushAll(tempQueue, aStarQueue);
				break;
			}
			else
				tempQueue.push(tempNode);
				
		}
	}
	return aStarQueue.front().origDirection;
}

