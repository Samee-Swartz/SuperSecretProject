#include "Pacman.h"
#include "Direction.h"
#include "Pawn.h"
#include "World.h"
#include <set>
#include <algorithm>
#include <climits>
#include <math.h>

#define Ks 1600
#define Kd 1096 // may need to adjust this. try making it bigger
#define Kg 1096

// keep track of explored nodes and give high reward to unexplored nodes

// the ghost pawns and a vector of blueGhosts. These are global because most functions
// use them and/or they only need to be filled in once.
std::vector<Pawn> ghosts;
std::vector<PathNode*> blueGhosts;

void FindBlueGhosts(const World& in_world);
int DepthFirst(const World& in_world, PathNode* in_curNode,
				  std::set<PathNode*> in_exploredNodes);
float CalculateHeuristic(const PathNode* in_node);

// Takes in world information, computes A* with a finite depth, and chooses a direction to move
Direction::Enum OnPacmanThink(const Pawn& in_ourPawn, const World& in_ourWorld,
							  float in_deltaTime, float in_totalTime) {
	// setup
	ghosts.clear();
	blueGhosts.clear();
	ghosts.push_back(in_ourWorld.GetInky());
	ghosts.push_back(in_ourWorld.GetBlinky());
	ghosts.push_back(in_ourWorld.GetPinky());
	ghosts.push_back(in_ourWorld.GetClyde());
	FindBlueGhosts(in_ourWorld);
	// explore
	// depth search in each direction and choose based on which has the shortest path to
	// a pointObj and highest ghost heuristic (on the next node from pacman)
	PathNode* curNode = in_ourWorld.GetNode((in_ourPawn.GetCurrentNode() == -1 ?
	                                        in_ourPawn.GetClosestNode() :
	                                        in_ourPawn.GetCurrentNode()));
	std::set<PathNode*> nodes;
	std::vector<std::pair<int, Direction::Enum> > counts;
	PathNodeConnection con;
	// depth first in all four directions
	for (int i=0; i < 4; i++) {	// loop through directions
		con = curNode->GetConnection((Direction::Enum)i); // get connection in that direction
		if (con.IsValid()) {	// is it valid?
			int s = CalculateHeuristic(in_ourWorld.GetNode(con.GetOtherNodeId())); // calculate heuristic of the node on the other end
			// run depthfirst on this new node. it will return a score for that node. add it to counts with the direction the node was in
			counts.push_back(std::make_pair(DepthFirst(in_ourWorld,
							 in_ourWorld.GetNode(con.GetOtherNodeId()), nodes) - s, (Direction::Enum)i));
		}
	}

	// how to rank. highest score is better
	auto func = [](const std::pair<int, Direction::Enum> a, const std::pair<int, Direction::Enum> b) {
		return a.first > b.first;
	};

	// if none of the directions worked... Shouldn't happen
	if (counts.size() < 1) {
		return Direction::Invalid;
	}
	// sort based on best direction
	std::sort(counts.begin(), counts.end(), func);

		// Which direction is the closest node in, from pacman
	Direction::Enum dirFromPacman;
	// unit vector from pacman to closest node
	Vector2 fromPacman = Vector2((curNode.node->GetPosition() - in_ourPawn.GetPosition()).x /
					   (curNode.node->GetPosition() - in_ourPawn.GetPosition()).Magnitude(),
					   (curNode.node->GetPosition() - in_ourPawn.GetPosition()).y /
					   (curNode.node->GetPosition() - in_ourPawn.GetPosition()).Magnitude());

	// This is to determine if pacman is facing the closest node. This affects how he should move
	if (fromPacman.x < .5 && fromPacman.x > -.5) { // unit vector's x is ~0
		if (fromPacman.y > .5)	// unit vector's y is ~1
			dirFromPacman = Direction::Up;
		else					// unit vector's y is ~ -1
			dirFromPacman = Direction::Down;
	} else if (fromPacman.x > .5) {	// unit vector's x is ~1
		dirFromPacman = Direction::Right;
	} else {					// unit vector's x is ~ -1
		dirFromPacman = Direction::Left;
	}
	// This decides if pacman needs to turn around first.
	/* the issue is that if he's facing left, the closest node is to the right of pacman, and
		the algorithm determines the next move should be down (from closest node), pacman first needs
		to turn toward closest node, before he can start heading down.
	*/
	if (dirFromPacman - in_ourPawn.GetFacingDirection() != 0) { // pacman is facing away from closest node
		if (in_ourPawn.GetFacingDirection() - counts[0].second == 0) // algorthims wants pacman to continue in his direction
			return counts[0].second; // do what algortihm said
		else	// algorithm wants pacman to move in a direction besides his current direction
			return dirFromPacman;	// turn toward the closest node
	} else {	// pacman is facing the closest node
		return counts[0].second;	// do what algorithm said
	}
}

// calculates the heuristic for the given node based on distance to all non-blue ghosts
float CalculateHeuristic(const PathNode* in_node) {
	float score = 0;
	// doesn't calculate for blue ghosts
	for (auto g : ghosts) {
		if (g.GetState() > 0)
			score += (std::exp((g.GetPosition() - in_node->GetPosition()).MagnitudeSqr()))/ Kg;
	}
	return score;
}

// adds any blue ghosts' locations to the vector
void FindBlueGhosts(const World& in_world) {
	for (auto g : ghosts) {
		if (g.GetState() < 0) {
			int nodes[5] = {-1,-1,-1,-1,-1};
			g.GetNextNodes(nodes);
			if (g.GetCurrentNode() != -1) {
				nodes[4] = g.GetCurrentNode();
			}
			for (int i = 0; i < 5; i++) {
				if (nodes[i] != -1)
					blueGhosts.push_back(in_world.GetNode(nodes[i]));
			}
		}
	}
}

// check's if a blue ghost is at the given location
bool FoundBlueGhost(const PathNode* in_node) {
	for (PathNode* n : blueGhosts) {
		if (in_node->Equals(n))
			return true;
	}
	return false;
}

// expands the current node in all four directions. Adds valid expansions to the frontier list
int DepthFirst(const World& in_world, PathNode* in_curNode,
				  std::set<PathNode*> in_exploredNodes) {
	if (in_exploredNodes.find(in_curNode) != in_exploredNodes.end()) {
		return INT_MAX; // hit a cycle
	}
	if (in_curNode->GetObject() != NULL) {
		return in_curNode->GetObject()->GetWorth()/Ks; // found a point object
	}
	if (FoundBlueGhost(in_curNode)) {
		return (pow(2, 5 - blueGhosts.size()) * 100)/Ks; // found a blue ghost
	}
	int count = INT_MAX;
	in_exploredNodes.insert(in_curNode); // add cur node to explored and keep looking

	PathNodeConnection con;
	for (int i=0; i < 4; i++) {
		con = in_curNode->GetConnection((Direction::Enum)i);
		if (con.IsValid()) {
			int c = DepthFirst(in_world, in_world.GetNode(con.GetOtherNodeId()),
							   in_exploredNodes);
			if (c > 0)
				count = std::min((float)count, c - (std::exp(con.GetCost())/Kd));
		}
	}
	return count;
}
