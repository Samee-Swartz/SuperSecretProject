#include "Pacman.h"
#include "Direction.h"
#include "Pawn.h"
#include "World.h"
#include <queue>

// score for first ghost
#define GHOST_SCORE 200
// tweak this to change how valuable pacman things objects are
#define OBJECT_WEIGHT 5

// the ghost pawns and a vector of blueGhosts. These are global because most functions
// use them and/or they only need to be filled in once.
std::vector<Pawn> ghosts;
std::vector<PathNode*> blueGhosts;

void FindBlueGhosts(const World& in_world);
void Expand(const World& in_world, ScoredNode in_curNode, std::priority_queue<ScoredNode>& out_frontier);

// Takes in world information, computes A* with a finite depth, and chooses a direction to move
Direction::Enum OnPacmanThink(const Pawn& in_ourPawn, const World& in_ourWorld,
                              float in_deltaTime, float in_totalTime)
{
	return Direction::Invalid;
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
	// compute curNode
	ScoredNode curNode = ScoredNode(in_ourWorld.GetNode(in_ourPawn.GetClosestNode()),
	                                0, 0, Direction::Invalid);
	// only search 30 moves ahead
	// tweak 30 to change search breadth
	for (int i = 0; i < 30; i++)
	{
		Expand(in_ourWorld, curNode, frontier);
		curNode = frontier.top();
		frontier.pop();
	}

	// Which direction is the closest node in, from pacman
	Direction::Enum dirFromPacman;
	// unit vector from pacman to closest node
	Vector2 fromPacman = (curNode.node->GetPosition() - in_ourPawn.GetPosition()).GetNormalizedVector();

	// This is to determine if pacman is facing the closest node. This affects how he should move
	if (fromPacman.x < .5 && fromPacman.x > -.5)
	{ // unit vector's x is ~0
		if (fromPacman.y > .5) // unit vector's y is ~1
			dirFromPacman = Direction::Up;
		else // unit vector's y is ~ -1
			dirFromPacman = Direction::Down;
	}
	else if (fromPacman.x > .5)
	{ // unit vector's x is ~1
		dirFromPacman = Direction::Right;
	}
	else
	{ // unit vector's x is ~ -1
		dirFromPacman = Direction::Left;
	}
	// This decides if pacman needs to turn around first.
	/* the issue is that if he's facing left, the closest node is to the right of pacman, and
		the algorithm determines the next move should be down (from closest node), pacman first needs
		to turn toward closest node, before he can start heading down.
	*/
	if (dirFromPacman - in_ourPawn.GetFacingDirection() != 0)
	{ // pacman is facing away from closest node
		if (in_ourPawn.GetFacingDirection() - curNode.origDirection == 0) // A* wants pacman to continue in his direction
			return curNode.origDirection; // follow what A* said
		else // A* wants pacman to move in a direction besides his current direction
			return dirFromPacman; // turn toward the closest node
	}
	else
	{ // pacman is facing the closest node
		return curNode.origDirection; // do what A* said
	}
}

// calculates the heuristic for the given node based on distance to all non-blue ghosts
float CalculateHeuristic(const PathNode* in_node)
{
	float score = 0;
	// doesn't calculate for blue ghosts
	for (auto g : ghosts)
	{ // loop through ghosts
		if (g.GetState() > 0) // check if they are not blue
			score += (g.GetPosition() - in_node->GetPosition()).MagnitudeSqr(); // add their distance from pacman
	}
	return score;
}

// adds any blue ghosts' locations (and surrounding nodes) to the vector
void FindBlueGhosts(const World& in_world)
{
	for (auto& g : ghosts)
	{ // loop through ghosts
		if (g.GetState() < 0)
		{ // if a ghost is blue
			int nodes[5] = {-1,-1,-1,-1,-1};
			g.GetNextNodes(nodes); // fill the closest nodes
			if (g.GetCurrentNode() != -1)
			{ // if there is a current node
				nodes[4] = g.GetCurrentNode(); // add that too
			}
			for (int i = 0; i < 5; i++)
			{ // for all of the positions we got
				if (nodes[i] != -1) // if they aren't invalid
					blueGhosts.push_back(in_world.GetNode(nodes[i])); // add the node to blueGhosts
			}
		}
	}
}

// check's if a blue ghost is at the given location
bool FoundBlueGhost(const PathNode* in_node)
{
	for (PathNode* n : blueGhosts)
	{ // loop through blue ghost nodes
		if (in_node->Equals(n)) // check if the node matches the current node
			return true;
	}
	return false;
}

// expands the current node in all four directions. Adds valid expansions to the frontier list
void Expand(const World& in_world, ScoredNode in_curNode, std::priority_queue<ScoredNode>& out_frontier)
{
	for (int i = 0; i < 4; i++)
	{ // check all four directions
		PathNodeConnection con = in_curNode.node->GetConnection(static_cast<Direction::Enum>(i)); // connection in that direction
		if (con.IsValid())
		{ // valid connection
			ScoredNode newNode;
			PathNode* p = in_world.GetNode(con.GetOtherNodeId()); // get next node
			// add connection cost to the cost to get to to current Node
			float score = con.GetCost() + in_curNode.arrivalAndNodeCost;
			if (p->GetObject() != NULL) // found an object
				score += p->GetObject()->GetWorth() * OBJECT_WEIGHT; // make it really desirable to get points
			if (FoundBlueGhost(p)) // found a blueghost
				score += pow(2, 5 - blueGhosts.size()) * 100; // calculates correct ghost score
			if (in_curNode.origDirection == Direction::Invalid) // if this is the first expansion off of pacman
			// origDirection should be the direction you moved this time
				newNode = ScoredNode(p, score, CalculateHeuristic(p), static_cast<Direction::Enum>(i));
			else // use the same origDirection
				newNode = ScoredNode(p, score, CalculateHeuristic(p), in_curNode.origDirection); // keep orig direction

			out_frontier.push(newNode); // add the valid new node to the frontier
		}
	}
}
