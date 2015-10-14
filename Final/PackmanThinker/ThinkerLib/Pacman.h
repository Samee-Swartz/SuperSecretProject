#include "Direction.h"

struct Pawn;
class World;

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

	// rate based on totalScore
	bool operator <(const ScoredNode& in_other) const {
		return totalScore < in_other.totalScore;
	}
	bool operator >(const ScoredNode& in_other) const {
		return totalScore > in_other.totalScore;
	}
};
