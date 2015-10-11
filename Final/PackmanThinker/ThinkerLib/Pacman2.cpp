#include "Pacman.h"
#include "Direction.h"
#include "Pawn.h"
#include "World.h"
#include <set>
#include <algorithm>
#include <climits>

#define Ks 1600
#define Kd 1096 // may need to adjust this. not straight line...
#define Kg 1096

// keep track of explored nodes and give high reward to unexplored nodes

// the ghost pawns and a vector of blueGhosts. There are global because most functions
// use them and/or they only need to be filled in once.
Pawn inky;
Pawn pinky;
Pawn blinky;
Pawn clyde;
std::vector<PathNode*> blueGhosts;
std::set<PathNode*> exploredNodes;

void FindBlueGhosts(const World& in_world);
int DepthFirst(const World& in_world, PathNode* in_curNode,
                  std::set<PathNode*> in_exploredNodes);

// Takes in world information, computes A* with a finite depth, and chooses a direction to move
Direction::Enum OnPacmanThink(const Pawn& in_ourPawn, const World& in_ourWorld,
                              float in_deltaTime, float in_totalTime) {
	// setup
	inky = in_ourWorld.GetInky();
	pinky = in_ourWorld.GetBlinky();
	blinky = in_ourWorld.GetPinky();
	clyde = in_ourWorld.GetClyde();
	FindBlueGhosts(in_ourWorld);
	if (in_ourWorld.GetNode(in_ourPawn.GetClosestNode())->GetObject() == NULL)
		exploredNodes.insert(in_ourWorld.GetNode(in_ourPawn.GetClosestNode()));
	// explore
	// depth search in each direction and choose based on which has the shortest path to
	// a pointObj and highest ghost heuristic (on the next node from pacman)
	PathNode* curNode = in_ourWorld.GetNode(in_ourPawn.GetClosestNode());
	PathNodeConnection up = curNode->GetConnection(Direction::Up);
	PathNodeConnection down = curNode->GetConnection(Direction::Down);
	PathNodeConnection left = curNode->GetConnection(Direction::Left);
	PathNodeConnection right = curNode->GetConnection(Direction::Right);
	std::set<PathNode*> nodes;
	std::vector<std::pair<int, Direction::Enum> > counts;
	std::pair<int, Direction::Enum> countUp, countDown, countRight, countLeft;
	if (up.IsValid()) {
		int s = CalculateHeuristic(in_ourWorld.GetNode(up.GetOtherNodeId()));
		counts.push_back(std::make_pair(DepthFirst(in_ourWorld,
		                 in_ourWorld.GetNode(up.GetOtherNodeId()), nodes) - s,
						Direction::Up));
	}
	if (down.IsValid()) {
		int s = CalculateHeuristic(in_ourWorld.GetNode(down.GetOtherNodeId()));
		counts.push_back(std::make_pair(DepthFirst(in_ourWorld,
		                 in_ourWorld.GetNode(down.GetOtherNodeId()), nodes) - s,
						Direction::Down));
	}
	if (right.IsValid()) {
		int s = CalculateHeuristic(in_ourWorld.GetNode(right.GetOtherNodeId()));
		counts.push_back(std::make_pair(DepthFirst(in_ourWorld,
		                 in_ourWorld.GetNode(right.GetOtherNodeId()), nodes) - s,
						Direction::Right));
	}
	if (left.IsValid()) {
		int s = CalculateHeuristic(in_ourWorld.GetNode(left.GetOtherNodeId()));
		counts.push_back(std::make_pair(DepthFirst(in_ourWorld,
		                 in_ourWorld.GetNode(left.GetOtherNodeId()), nodes) - s,
						Direction::Left));
	}

	auto func = [](const std::pair<int, Direction::Enum> a, const std::pair<int, Direction::Enum> b) {
		return a.first > b.first;
	};

	if (counts.size() < 1) {
		return Direction::Invalid;
	}
	std::sort(counts.begin(), counts.end(), func);
	return counts[0].second;
}

// calculates the heuristic for the given node based on distance to all non-blue ghosts
float CalculateHeuristic(const PathNode* in_node) {
	float score = 0;
	// doesn't calculate for blue ghosts
	if (inky.GetState() > 0)
		score += (std::exp(inky.GetPosition() - in_node->GetPosition()))/ Kg;

	if (blinky.GetState() > 0)
		score += (std::exp(blinky.GetPosition() - in_node->GetPosition()))/ Kg;

	if (pinky.GetState() > 0)
		score += (std::exp(pinky.GetPosition() - in_node->GetPosition()))/ Kg;

	if (clyde.GetState() > 0)
		score += (std::exp(clyde.GetPosition() - in_node->GetPosition()))/ Kg;

	return score;
}

// adds any blue ghosts' locations to the vector
void FindBlueGhosts(const World& in_world) {
	if (inky.GetState() < 0)
		blueGhosts.push_back(in_world.GetNode(inky.GetClosestNode()));

	if (blinky.GetState() < 0)
		blueGhosts.push_back(in_world.GetNode(blinky.GetClosestNode()));

	if (pinky.GetState() < 0)
		blueGhosts.push_back(in_world.GetNode(pinky.GetClosestNode()));

	if (clyde.GetState() < 0)
		blueGhosts.push_back(in_world.GetNode(clyde.GetClosestNode()));
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
		return in_curNode->GetObject()->GetWeight()/Ks; // found a point object
	}
	if (FoundBlueGhost(in_curNode)) {
		return (std::pow(2, 5 - blueGhosts.size()) * 100)/Ks; // found a blue ghost
	}
	int count = INT_MAX;
	in_exploredNodes.insert(in_curNode); // add cur node to explored and keep looking
	// check all directions
	PathNodeConnection up = in_curNode->GetConnection(Direction::Up);
	PathNodeConnection down = in_curNode->GetConnection(Direction::Down);
	PathNodeConnection left = in_curNode->GetConnection(Direction::Left);
	PathNodeConnection right = in_curNode->GetConnection(Direction::Right);
	if (up.IsValid()) {
		int c = DepthFirst(in_world, in_world.GetNode(up.GetOtherNodeId()),
		                   in_exploredNodes);
		if (c > 0)
			count = std::min(count, c - (std::exp(up.GetCost())/Kd));
	}
	if (down.IsValid()) {
		int c = DepthFirst(in_world, in_world.GetNode(down.GetOtherNodeId()),
		                   in_exploredNodes);
		if (c > 0)
			count = std::min(count, c - (std::exp(down.GetCost())/Kd));
	}
	if (right.IsValid()) {
		int c = DepthFirst(in_world, in_world.GetNode(right.GetOtherNodeId()),
		                   in_exploredNodes);
		if (c > 0)
			count = std::min(count, c - (std::exp(right.GetCost())/Kd));
	}
	if (left.IsValid()) {
		int c = DepthFirst(in_world, in_world.GetNode(left.GetOtherNodeId()),
		                   in_exploredNodes);
		if (c > 0)
			count = std::min(count, c - (std::exp(left.GetCost())/Kd));
	}
	return count;
}
