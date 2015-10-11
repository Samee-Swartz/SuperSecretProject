#include "Pacman.h"
#include "Direction.h"
#include "Pawn.h"
#include "World.h"
#include <set>
#include <algorithm>
#include <climits>
#include <math.h>

#define Ks 1600
#define Kd 1096 // may need to adjust this. not straight line...
#define Kg 1096

// keep track of explored nodes and give high reward to unexplored nodes

// the ghost pawns and a vector of blueGhosts. There are global because most functions
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
	for (int i=0; i < 4; i++) {
		con = curNode->GetConnection((Direction::Enum)i);
		if (con.IsValid()) {
			int s = CalculateHeuristic(in_ourWorld.GetNode(con.GetOtherNodeId()));
			counts.push_back(std::make_pair(DepthFirst(in_ourWorld,
							 in_ourWorld.GetNode(con.GetOtherNodeId()), nodes) - s, (Direction::Enum)i));
		}
	}

	auto func = [](const std::pair<int, Direction::Enum> a, const std::pair<int, Direction::Enum> b) {
		return a.first > b.first;
	};

	if (counts.size() < 1) {
		return Direction::Invalid;
	}
	std::sort(counts.begin(), counts.end(), func);

	// Which direction is the closest node in, from pacman
	Direction::Enum dirToPacman;
	Vector2 toPacman = Vector2((curNode->GetPosition() - in_ourPawn.GetPosition()).x /
					   (curNode->GetPosition() - in_ourPawn.GetPosition()).Magnitude(),
					   (curNode->GetPosition() - in_ourPawn.GetPosition()).y /
					   (curNode->GetPosition() - in_ourPawn.GetPosition()).Magnitude());

	if (toPacman.x < .5 && toPacman.x > -.5) { // x == 0
		if (toPacman.y > .5)
			dirToPacman = Direction::Up;
		else
			dirToPacman = Direction::Down;
	} else if (toPacman.x > .5) {
		dirToPacman = Direction::Right;
	} else {
		dirToPacman = Direction::Left;
	}
	// may have to turn around to face correct direction
	if (dirToPacman - in_ourPawn.GetFacingDirection() != 0) { // pacman is facing away from closest node
		if (in_ourPawn.GetFacingDirection() - counts[0].second == 0) // move in pacman's direction
			return counts[0].second;
		else
			return dirToPacman;
	} else {
		return counts[0].second;
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
