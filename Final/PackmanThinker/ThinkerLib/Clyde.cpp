#include "Clyde.h"
#include "Pawn.h"
#include "World.h"
#include "Pacman.h"
#include <stdlib.h>


//TODO: Determine these values (Benny? <3) 

#define SW_botL 5 //this is the node ID oat the bottom left corner of the board
#define SW_midR 6 //node on the far right of the SW loop, top
#define SW_topR 7 //node on the top row of the loop, on the right
#define SW_topL 8 //node on the top row of the loop, on the left


/* Clyde
chase algorithm: simulated annealing
goal: 4 in front of pacman

/* Scatter goal : SW corner 
 * 
 *    LL   LM   RM   RR
 *         x    x        top
 *         x    x
 *    x    x    x    x   mid
 * 
 *    x    x    x    x   bot 
 */

#if 0
	Direction::Enum OnClydeThink(const Pawn& in_ourPawn, const World& in_ourWorld, float in_deltaTime, float in_totalTime)
{
	int SW_top = in_ourWorld.GetNode(SW_topR)->GetPosition().x;
	int SW_mid = in_ourWorld.GetNode(SW_midR)->GetPosition().y;
	int SW_bot = in_ourWorld.GetNode(SW_botL)->GetPosition().y;
	int SW_LL = in_ourWorld.GetNode(SW_botL)->GetPosition().x;
	int SW_LM = in_ourWorld.GetNode(SW_topL)->GetPosition().x;
	int SW_RM = in_ourWorld.GetNode(SW_topR)->GetPosition().x;
	int SW_RR = in_ourWorld.GetNode(SW_midR)->GetPosition().x;
	
	PathNode* goalNode = nullptr;
	
//Store the direction we are currently facing
	Direction::Enum curDir = in_ourPawn.GetFacingDirection();
	
// compute curNode
	ScoredNode curNode = ScoredNode(in_ourWorld.GetNode((in_ourPawn.GetCurrentNode() == -1 ?
									in_ourPawn.GetClosestNode() : in_ourPawn.GetCurrentNode())),
									0, 0, Direction::Invalid);
	
	ScoredNode pacNode = ScoredNode(in_ourWorld.GetNode((in_ourWorld.GetPacman().GetCurrentNode() == -1 ?
									in_ourWorld.GetPacman().GetClosestNode() : in_ourWorld.GetPacman().GetCurrentNode())),
									0, 0, Direction::Invalid); 
	
/* If in frighten mode, determine if we are at an intersection. If we are not, then continue in the current direction.
	 * else, randomly select a direction from the available directions. */
	if(in_ourPawn.GetState() == -1){
		
//if we are currently not at a node, continue in the direction we are going
		if(in_ourPawn.GetCurrentNode() != 1)
			return curDir;
		
//if we are currently at a node, find which directions are available for turning, and pick a random one
		else{
			std::vector<Direction::Enum> possibleDirs;
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
	
/* Mode: scatter
	 * If Clyde is not in the SW corner, then the goal is the SW corner of the board (SW_LL, SW_bot)
	 * If Clyde is in the SW corner loop, then the goal (dir) is (towards) the next node in the loop 
	 */
	else if(in_ourPawn.getState() == 2){
		
		
		int myX = curNode.getLocation().x;
		int myZ = curNode.getLocation().y;
//If we are in the loop, move in the dir corresponding to its position.
//if on the very left,  at or below SW_mid and above SW_bot, move down
		if(myX == SW_LL && myY <= SW_mid && myY > SW_bot)
			return Direction::Down;		
		
//if on the very botton, right or at SW_LL and left of SW_RR, move right
		if(myY == SW_bot && myX <= SW_LL && myX > SW_RR)
			return Direction::Right;
		
//if on the far right, at or above SW_bot and below SW_mid, move up
		if(myX == SW_RR && myY >= SW_bot && myY < SW_mid)
			return Direction::Up;
		
//if on the right horiz ledge, at or to the left of SW_RR and to the right of SW_RM, go left
		if(myY == SW_mid && myX <= SW_RR && myX > SW_RM)
			return Direction::Left;
		
//if on the right vert ledge, at or above SW_mid and below SW_top, move up
		if(myX == SW_RM && myY >= SW_mid && myY < SW_top)
			return Direction::Up;
		
//if on the top ledge, at or to the left of SW_RM and to the right of SW_LM, move left
		if(myY == SW_top && myX <= SW_RM && myX > SW_LM)
			return Direction::Left;
		
//if on the left vert ledge, at or below SW_top and above SW_mid, move down 
		if(myX == SW_LM && myY <= SW_top && myY > SW_mid)
			return Direction::Down;
		
//if on the left horiz ledge, at or to the left of SW_LM and to the right of SW_LL, move left
		if(myY == SW_mid && myX <= SW_LM && myY > SW_LL)
			return Direction::Left;

/* we are outside of the loop 
		 * Use A* with a goal of reaching the top left corner (NW_0x, NW_0y)
		 */
		else{
//prep for A*
			std::priority_queue<ScoredNode> frontier;
			
//Goal node
			goalNode = in_world.getNode(SW_botL);
			
//Begin copy from Samee's Pacman.cpp
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
	
	
/* Mode: Chase 
	 * goal: 4 in front of Pacman
	 * algorithm: simulated annealing
	 * 
	 */
	else if(in_ourPawn.getState() == 2){
		
		PathNodeConnection con = pacNode.node.GetConnection(pacDir);
		PathNode* inFront;
		
		if(!con.isValid()){
				goalNode = pacNode; 
		}
		else{
			for(int i = 0; i < 4; i++){
					inFront = in_world.GetNode(con.GetOtherNodeId());
					con = inFront->node.getConnection(pacDir);
					
					if(!con.isValid()){
						goalNode = *inFront;
					}else{
						goalNode = *in_world.GetNode(con.GetOtherNodeId());
					}
				
				}
		}
		
		
/*Simulated annealing */ 
		float randomness = (100 / in_totalTime) % 100; //gradually decreases as time goes on, stays less than 100 
		if(rand() % 100 > randomness){
//choose a random valid direction
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
		else{
//run greedy best for the next node
			
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
			} //end for loop
			return bestDir;
		}
		
		
		
	}
	else //the state is not chase or frightened or scatter
		return Direction::Invalid;
	
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
#endif
