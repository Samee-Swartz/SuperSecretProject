#include "Pacman.h"
#include "Direction.h"
#include "Pawn.h"
#include "World.h"

#define GHOST_SCORE 500

/*
enum Enum
{
Dot,
SuperDot,
Fruit
};
*/


// A* with goal of most points - heuristic is distance from ghosts

// google pacman strategies
// A*

// the ghost pawns and a vector of blueGhosts. There are global because most functions
// use them and/or they only need to be filled in once.
Pawn* inky;
Pawn* pinky;
Pawn* blinky;
Pawn* clyde;
vector<PathNode*> blueGhosts;

struct ScoredNode {
	PathNode* node;
	float arrivalAndNodeCost;
	float totalScore;
	Direction::Enum origDirection;

	ScoredNode(PathNode* m_node, float m_aanc, float m_heuristic, Direction::Enum m_dir) {
		node = m_node;
		origDirection = m_dir;
		arrivalAndNodeCost = m_aanc;
		totalScore = m_aanc + m_heuristic;
	}

	bool operator <(const ScoredNode& in_other) {
		return totalScore < in_other.totalScore;
	}
	bool operator >(const ScoredNode& in_other) {
		return totalScore > in_other.totalScore;
	}
};

Direction::Enum OnPacmanThink(const Pawn& in_ourPawn, const World& in_ourWorld,
                              float in_deltaTime, float in_totalTime) {
	inky = in_ourWorld.GetInky();
	pinky = in_ourWorld.GetBlinky();
	blinky = in_ourWorld.GetPinky();
	clyde = in_ourWorld.GetClyde();
	FindBlueGhosts();
	std::PriorityQueue<ScoredNode> frontier;
	ScoredNode curNode = ScoredNode(in_ourWorld.GetNode(in_ourPawn->GetClosestNode()),
	                                0, 0, Direction::Invalid);
	for (int i=0; i < 30; i++) {
		Expand(in_ourWorld, curNode, frontier);
		curNode = frontier.top();
		frontier.pop();
	}
	return curNode.origDirection;
}

float CalculateHeuristic(const PathNode* in_node) {
	float score = 0;
	// doesn't calculate for blue ghosts
	if (inky.GetState > 0)
		score += (inky.GetPosition() - in_node->GetPosition()).MagnitudeSqr();

	if (blinky.GetState > 0)
		score += (blinky.GetPosition() - in_node->GetPosition()).MagnitudeSqr();

	if (pinky.GetState > 0)
		score += (pinky.GetPosition() - in_node->GetPosition()).MagnitudeSqr();

	if (clyde.GetState > 0)
		score += (clyde.GetPosition() - in_node->GetPosition()).MagnitudeSqr();

	return score;
}

void FindBlueGhosts() {
	if (inky.GetState < 0)
		blueGhosts.push_back(inky.GetClosestNode());

	if (blinky.GetState < 0)
		blueGhosts.push_back(blinky.GetClosestNode());

	if (pinky.GetState < 0)
		blueGhosts.push_back(pinky.GetClosestNode());

	if (clyde.GetState < 0)
		blueGhosts.push_back(clyde.GetClosestNode());
}

void Expand(const World& in_world, ScoredNode* in_curNode, std::PriorityQueue<ScoredNode>& out_frontier) {
	PathNodeConnection up = in_curNode.node->GetConnection(Direction::Up);
	PathNodeConnection down = in_curNode.node->GetConnection(Direction::Down);
	PathNodeConnection left = in_curNode.node->GetConnection(Direction::Left);
	PathNodeConnection right = in_curNode.node->GetConnection(Direction::Right);
	// score = cost of path + ghost heuristics to node + cost getting to prevNode
	// 		   + object's worth + blue ghost (if he's there)
	if (up.IsValid()) {
		ScoredNode newNode;
		PathNode* p = in_world.GetNode(up.GetOtherNodeId());
		float score = up.GetCost() + CalculateHeuristic(p) + in_curNode.arrivalAndNodeCost;
		if (p->GetObject() != NULL)
			score += p->GetObject()->GetWorth();
		if (FoundBlueGhost(p))
			score += GHOST_SCORE;
		if (in_curNode.origDirection == Direction::Invalid)
			newNode = ScoredNode(p, score, Direction::Up);
		else
			newNode = ScoredNode(p, score, in_curNode.origDirection);

		out_frontier.push(newNode);
	}

	if (down.IsValid()) {
		ScoredNode newNode;
		PathNode* p = in_world.GetNode(down.GetOtherNodeId());
		float score = down.GetCost() + CalculateHeuristic(p) + in_curNode.arrivalAndNodeCost;
		if (p->GetObject() != NULL)
			score += p->GetObject()->GetWorth();
		if (FoundBlueGhost(p))
			score += GHOST_SCORE;
		if (in_curNode.origDirection == Direction::Invalid)
			newNode = ScoredNode(p, score, Direction::Down);
		else
			newNode = ScoredNode(p, score, in_curNode.origDirection);

		out_frontier.push(newNode);
	}

	if (left.IsValid()) {
		ScoredNode newNode;
		PathNode* p = in_world.GetNode(left.GetOtherNodeId());
		float score = left.GetCost() + CalculateHeuristic(p) + in_curNode.arrivalAndNodeCost;
		if (p->GetObject() != NULL)
			score += p->GetObject()->GetWorth();
		if (FoundBlueGhost(p))
			score += GHOST_SCORE;
		if (in_curNode.origDirection == Direction::Invalid)
			newNode = ScoredNode(p, score, Direction::Left);
		else
			newNode = ScoredNode(p, score, in_curNode.origDirection);

		out_frontier.push(newNode);
	}

	if (right.IsValid()) {
		ScoredNode newNode;
		PathNode* p = in_world.GetNode(right.GetOtherNodeId());
		float score = right.GetCost() + CalculateHeuristic(p) + in_curNode.arrivalAndNodeCost;
		if (p->GetObject() != NULL)
			score += p->GetObject()->GetWorth();
		if (FoundBlueGhost(p))
			score += GHOST_SCORE;
		if (in_curNode.origDirection == Direction::Invalid)
			newNode = ScoredNode(p, score, Direction::Right);
		else
			newNode = ScoredNode(p, score, in_curNode.origDirection);

		out_frontier.push(newNode);
	}
}

bool FoundBlueGhost(const PathNode* in_node) {
	for (auto n : blueGhosts) {
		if (in_node->Equals(n))
			return true;
	}
	return false;
}
