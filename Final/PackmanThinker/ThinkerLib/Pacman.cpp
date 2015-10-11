#include "Pacman.h"
#include "Direction.h"
#include "Pawn.h"
#include "World.h"
#include <queue>

#define GHOST_SCORE 200
#define OBJECT_WEIGHT 5

// the ghost pawns and a vector of blueGhosts. There are global because most functions
// use them and/or they only need to be filled in once.
std::vector<Pawn> ghosts;
std::vector<PathNode*> blueGhosts;

// Keeps track of node information during A* computation.
struct ScoredNode {
	PathNode* node;
	float arrivalAndNodeCost; // arrival + curNode
	float totalScore; // arrival + curNode + heuristic
	// the first direction taken (leaving pacman's current position) to get to this node
	Direction::Enum origDirection;

	ScoredNode(PathNode* m_node, float m_aanc, float m_heuristic, Direction::Enum m_dir) {
		node = m_node;
		origDirection = m_dir;
		arrivalAndNodeCost = m_aanc;
		totalScore = m_aanc + m_heuristic;
	}

	ScoredNode() {}

	bool operator <(const ScoredNode& in_other) const {
		return totalScore < in_other.totalScore;
	}
	bool operator >(const ScoredNode& in_other) const {
		return totalScore > in_other.totalScore;
	}
};

void FindBlueGhosts(const World& in_world);
void Expand(const World& in_world, ScoredNode in_curNode, std::priority_queue<ScoredNode>& out_frontier);

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
	// A*
	std::priority_queue<ScoredNode> frontier;
	ScoredNode curNode = ScoredNode(in_ourWorld.GetNode((in_ourPawn.GetCurrentNode() == -1 ?
                                    in_ourPawn.GetClosestNode() : in_ourPawn.GetCurrentNode())),
	                                0, 0, Direction::Invalid);
	// only search 30 moves ahead
	for (int i=0; i < 30; i++) {
		Expand(in_ourWorld, curNode, frontier);
		curNode = frontier.top();
		frontier.pop();
	}

	// Which direction is the closest node in, from pacman
	Direction::Enum dirToPacman;
	Vector2 toPacman = Vector2((curNode.node->GetPosition() - in_ourPawn.GetPosition()).x /
					   (curNode.node->GetPosition() - in_ourPawn.GetPosition()).Magnitude(),
					   (curNode.node->GetPosition() - in_ourPawn.GetPosition()).y /
					   (curNode.node->GetPosition() - in_ourPawn.GetPosition()).Magnitude());

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
		if (in_ourPawn.GetFacingDirection() - curNode.origDirection == 0) // move in pacman's direction
			return curNode.origDirection;
		else
			return dirToPacman;
	} else {
		return curNode.origDirection;
	}
}

// calculates the heuristic for the given node based on distance to all non-blue ghosts
float CalculateHeuristic(const PathNode* in_node) {
	float score = 0;
	// doesn't calculate for blue ghosts
	for (auto g : ghosts) {
		if (g.GetState() > 0)
			score += (g.GetPosition() - in_node->GetPosition()).MagnitudeSqr();
	}
	return score;
}

// adds any blue ghosts' locations (and surrounding nodes) to the vector
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
void Expand(const World& in_world, ScoredNode in_curNode, std::priority_queue<ScoredNode>& out_frontier) {

	for (int i=0; i < 4; i++) {
		PathNodeConnection con = in_curNode.node->GetConnection((Direction::Enum)i);
		if (con.IsValid()) {
			ScoredNode newNode;
			PathNode* p = in_world.GetNode(con.GetOtherNodeId());
			float score = con.GetCost() + in_curNode.arrivalAndNodeCost;
			if (p->GetObject() != NULL) // found an object
				score += p->GetObject()->GetWorth()*OBJECT_WEIGHT; // make it really desirable to get points
			if (FoundBlueGhost(p)) // found a blueghost
				score += pow(2, 5 - blueGhosts.size()) * 100;
			if (in_curNode.origDirection == Direction::Invalid) // first expansion
				newNode = ScoredNode(p, score, CalculateHeuristic(p), (Direction::Enum)i);
			else // use the same origDirection
				newNode = ScoredNode(p, score, CalculateHeuristic(p), in_curNode.origDirection);

			out_frontier.push(newNode);
		}
	}
}
