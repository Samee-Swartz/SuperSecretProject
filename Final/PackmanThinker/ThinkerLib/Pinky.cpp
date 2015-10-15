#include "Pinky.h"
#include "Pawn.h"
#include "World.h"
#include "Pacman.h"


//TODO: Determine these values (Benny? <3) 

#define NW_topL = 5; //this is the node ID of the node on the very top left corner of the board
#define NW_botR = 6; //node ID of the node on the bottom right of the NW loop 

/* Pinky
chase mode
algorithm: greedy first
goal: 2 in front of pacman

scatter mode
goal: NorthWest corner 

Frighten mode
no goal, at any intersection decide a random direction*/ 


Direction::Enum OnPinkyThink(const Pawn& in_ourPawn, const World& in_ourWorld, float in_deltaTime, float in_totalTime){
	
	//Gather the x and y points of the reference nodes 
	int NW_0x = NW_topL.getPosition().x;
	int NW_0y = NW_topL.getPosition().y;
	int NW_x = NW_botR.getPosition().x;
	int NW_y = NW_botR.getPosition().y;
	
	//goalNode for reference in scatter and chase
	PathNode goalNode;
	
	//Store the direction we are currently facing
	Direction::Enum curDir = in_ourPawn.GetFacingDirection();
	
	// compute curNode
	ScoredNode curNode = ScoredNode(in_ourWorld.GetNode((in_ourPawn.GetCurrentNode() == -1 ?
                                    in_ourPawn.GetClosestNode() : in_ourPawn.GetCurrentNode())),
	                                0, 0, Direction::Invalid);
	
	ScoredNode pacNode = ScoredNode(in_ourWorld.GetNode((in_ourWorld.getPacman().GetCurrentNode() == -1 ?
									in_ourWorld.getPacman().GetClosestNode() : in_ourWorld.getPacman().GetCurrentNode())),
									0, 0, Direction::Invalid); 
	
	/* If in frighten mode, determine if we are at an intersection. If we are not, then continue in the courrent direction.
	 * else, randomly select a direction from the available directions. */
	if(in_ourPawn.GetState() == -1){
		
		//if we are currently not at a node, continue in the direction we are going
		if(in_ourPawn.getCurrentNode() != 1)
			return curDir;
		
		//if we are currently at a node, find which directions are available for turning, and pick a random one
		else{
			vector<Direction::Enum> possibleDirs;
			int randDir;
			
			PathNodeConnection con;
			//Search in all four directions for a connected node
			for (int i=0; i < 4; i++) {	// loop through directions
				con = curNode->GetConnection((Direction::Enum)i); // get connection in that direction
				if (con.IsValid()) {	// is it valid?
						possibleDirs.push_back((Direction::Enum)i);
				}
			}
			
			//possibleDirs now holds then number of connections at the current node 
			randDir = rand() % possibleDirs.size();
			return possibleDirs[randDir];
		}
	}
	
	/* Chase Mode. Determine the point which is 2 spaces in front of Pacman. If there are not 2 open spaces available in front of Pacman,
	 * then use the furthest (up to 2) connected node in that direction as the goal node. Then, use greedy first to decide which node to move towards next,
	 * calculate heuristic by the distance between that node and the goal node. 
	 */
	else if(in_ourPawn.getState() == 1){
		Vector2 goalLoc;
		
		Enum::Direction pacDir = in_world.getPacman().getFacingDirection();
		
		//set the goal as Pacman's location if there is no node in front of it
		//if there is one node in front of pacman, look in front to see if there is one connected. 
		//if not, use connected node. If yes, use node 2 connections in front of pacman
		PathNodeConnection con = pacNode.node.GetConnection(pacDir);
		
		if(!con.isValid()){
			goalNode = pacNode; 
		}else{ //there is at least one connection 
			
			PathNode* inFront = in_world.GetNode(con.GetOtherNodeId());
			con = inFront->node.getConnection(pacDir);
			
			if(!con.isValid()){
				goalNode = *inFront;
			}else{
				goalNode = *in_world.GetNode(con.GetOtherNodeID());
			}
		}
		
		/* Now we have the goalNode. Determine the direction we should go depending on
		 * the location of Pinky and the best node that it is closest to. 
		 */
		Direction::Enum bestDir = Invalid;
		float bestHeuristic = 1000; //dummy value, any valid score would be less
		
		/* For each possible direction, check if there's a connection. If there's a valid
		 * connection, the connected node's score with the best score already. if better, save it
		 */
		for(int i = 0; i++; i < 4){
			PathNodeConnection con = curNode.node.node->GetConnection((Direction::Enum)i);
			if(con.isValid()){
				PathNode* p = in_world.GetNode(con.GetOtherNodeId()); 
				float score = CalculateHeuristic(p);
				
				if(score > bestHeuristic){
					bestHeuristic = score;
					bestDir = Direction::Enum(i);
				}
				
			}

		}
		
		//return the direction corresponding to the best score
		return bestDir;
	}
	
	/* Scatter mode. Determine if we are in the NW corner. If we are, then continue until hit an intersection,
	 * and turn left. If we are not in the NW corner, then move in that direction. 
	 * NOTE: This assumes (0,0) is the top left corner of the board. Therefore (NW_x, NW_y) is the lower right 
	 * corner of the loop that Pinky will move around.*/ 
	else if(in_ourPawn.getState() == 2){
		
		//If we are in the loop, move in the dir corresponding to its position.
		//if x is NW_0x and y is less than NW_y, move downwards
		if(curNode.getPosition().x == NW_0x && curNode.getPosition().y < NW_y)
			return Direction::Down;
									
		//if y is NW_y and x is less than NW_x, move to the right
		else if(curNode.getPosition().y == NW_y && curNode.getPosition().x < NW_x)
			return Direction::Right;
		
		//if x is NW_x and y is greater than NW_0y and y is smaller than or equal to NW_y, move upwards
		else if(curNode.getPosition().x == NW_x && curNode.getPosition().y > NW_0y && curNode.getPosition().y <= NW_y)
			return Direction::Up;
		
		//if y is NW_0y and x is greater than NW_0x and x is smaller than or equal to NW_x,  then move to the left 
		else if(curNode.getPosition().y == NW_0y && curNode.getPosition().x > NW_0x && curNode.getPosition().x <= NW_x)
			return Direction::Left;
		
		/* we are outside of the loop 
		 * Use A* with a goal of reaching the top left corner (NW_0x, NW_0y)
		 */
		else{
			//prep for A*
			std::priority_queue<ScoredNode> frontier;
			
			//Goal node
			goalNode = in_world.getNode(NW_topL);
			
			//Begin copy from Samee's Pacman.coo
			// Which direction is the closest node in, from pacman
			Direction::Enum dirfromPinky;
			// unit vector from pacman to closest node
			Vector2 fromPinky = Vector2((curNode.node->GetPosition() - in_ourPawn.GetPosition()).x /
							   (curNode.node->GetPosition() - in_ourPawn.GetPosition()).Magnitude(),
							   (curNode.node->GetPosition() - in_ourPawn.GetPosition()).y /
							   (curNode.node->GetPosition() - in_ourPawn.GetPosition()).Magnitude());

			// This is to determine if pacman is facing the closest node. This affects how he should move
			if (fromPinky.x < .5 && fromPinky.x > -.5) { // unit vector's x is ~0
				if (fromPinky.y > .5)	// unit vector's y is ~1
					dirfromPinky = Direction::Up;
				else					// unit vector's y is ~ -1
					dirfromPinky = Direction::Down;
			} else if (fromPinky.x > .5) {	// unit vector's x is ~1
				dirfromPinky = Direction::Right;
			} else {					// unit vector's x is ~ -1
				dirfromPinky = Direction::Left;
			}
			// This decides if pacman needs to turn around first.
			/* the issue is that if he's facing left, the closest node is to the right of pacman, and
				the algorithm determines the next move should be down (from closest node), pacman first needs
				to turn toward closest node, before he can start heading down.
			*/
			if (dirfromPinky - in_ourPawn.GetFacingDirection() != 0) { // pacman is facing away from closest node
				if (in_ourPawn.GetFacingDirection() - curNode.origDirection == 0) // A* wants pacman to continue in his direction
					return curNode.origDirection;		// follow what A* said
				else		// A* wants pacman to move in a direction besides his current direction
					return dirfromPinky;	// turn toward the closest node
			} else {	// pacman is facing the closest node
				return curNode.origDirection;	// do what A* said
			}
			//end copy from Samee's Pacman.cpp
		}
		
	}
	else 
		return Direction::Invalid; //Not a valid mode, returns Invalid for a direction
	
}

// expands the current node in all four directions. Adds valid expansions to the frontier list
void Expand(const World& in_world, ScoredNode in_curNode, std::priority_queue<ScoredNode>& out_frontier) {

	for (int i=0; i < 4; i++) {	// check all four directions
		PathNodeConnection con = in_curNode.node->GetConnection((Direction::Enum)i);	// connection in that direction
		if (con.IsValid()) {	// valid connection
			ScoredNode newNode;
			PathNode* p = in_world.GetNode(con.GetOtherNodeId());	// get next node
			// add connection cost to thfe cost to get to to current Node
			float score = con.GetCost() + in_curNode.arrivalAndNodeCost;
		
			if (in_curNode.origDirection == Direction::Invalid) // if this is the first expansion off of Pinky
				// origDirection should be the direction you moved this time
				newNode = ScoredNode(p, score, CalculateHeuristic(p), (Direction::Enum)i);
			else // use the same origDirection
				newNode = ScoredNode(p, score, CalculateHeuristic(p), in_curNode.origDirection); // keep orig direction

			out_frontier.push(newNode);	// add the valid new node to the frontier
		}
	}
}


// calculates the heuristic for the given node based on distance to the goal node
float CalculateHeuristic(const PathNode* in_node) {
	return sqrt(in_node->getPosition().x * pacNode.node->getPosition().x + in_node->getPosition().y * pacNode.node->getPosition().y;
}
