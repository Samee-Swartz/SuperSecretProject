#include "Blinky.h"
#include "Pawn.h"
#include "World.h"
#include <queue>

#define SCATTER_NODE 416015878
#define FRIGHTEN_NODE 416015878
#define HOUSE_NODE 1132635177
#include <unordered_set>
#include <memory>

// Keeps track of node information during A* computation.
struct ScoredNode
{
	PathNode* node; // current node

	// cost to get to this node (all previous travel + connectionCost) + curNodeValue if applicable
	float arrivalAndNodeCost;
	float totalScore; // arrivalAndNodeCost + heuristic
	// the first direction taken (leaving pacman's current position) to get to this node
	Direction::Enum connectionDirection;
	std::shared_ptr<ScoredNode> prev;

	ScoredNode(PathNode* in_node, float in_aanc, float in_heuristic, Direction::Enum in_dir, std::shared_ptr<ScoredNode> in_prev)
	{
		node = in_node;
		connectionDirection = in_dir;
		arrivalAndNodeCost = in_aanc;
		totalScore = in_aanc + in_heuristic;
		prev = in_prev;
	}

	ScoredNode()
	{
		node = nullptr;
		connectionDirection = Direction::Invalid;
		arrivalAndNodeCost = 0;
		totalScore = 0;
		prev = std::shared_ptr<ScoredNode>(nullptr);
	}

	~ScoredNode()
	{
		if(prev.get())
			prev.reset();
	}

	bool IsRoot() const
	{
		return !prev.get();
	}

	ScoredNode* GetRoot()
	{
		if (IsRoot())
			return this;
		return prev->GetRoot();
	}

	ScoredNode* GetRootConnection()
	{
		if (IsRoot())
			return nullptr;
		if (prev->IsRoot())
			return this;
		return prev->GetRootConnection();
	}
};

//Calculates the straight line distance between the currently node in examination and the target node
float crowDist(const PathNode& aNode, const PathNode& bNode)
{
	return (bNode.GetPosition() - aNode.GetPosition()).Magnitude();
}


//pushes all nodes from queue A into queue B
void pushAll(std::queue<ScoredNode>& queueA, std::queue<ScoredNode>& queueB)
{
	for (int i = 0; i <= queueA.size(); i++)
	{
		ScoredNode aTemp = queueA.front();
		queueA.pop();
		queueB.push(aTemp);
	}
}

bool NodeComparer(std::shared_ptr<ScoredNode> in_a, std::shared_ptr<ScoredNode> in_b)
{
	return in_a->totalScore < in_b->totalScore;
}

// expands the current node in all four directions. Adds valid expansions to the frontier list
void Expand(const World& in_world,
	std::shared_ptr<ScoredNode> in_curNode,
	const PathNode& in_targetNode,
	std::priority_queue<std::shared_ptr<ScoredNode>, std::function<bool(std::shared_ptr<ScoredNode>, std::shared_ptr<ScoredNode>)>>& inout_frontier,
	std::map<PathNode*, std::shared_ptr<ScoredNode>>& inout_frontierMap,
	const std::map<PathNode*, std::shared_ptr<ScoredNode>>& in_processedNodes)
{
	for (int i = 0; i < 4; i++)
	{
		const PathNodeConnection& connection = in_curNode->node->GetConnection(static_cast<Direction::Enum>(i));
		if(connection.IsValid())
		{
			//Gather the information on this next connection
			PathNode* connectedNode = in_world.GetNode(connection.GetOtherNodeId());

			//If the node has already been processed then by definition of A* we must be back tracking and we can ignore this connection
			auto foundInProcessed = in_processedNodes.find(connectedNode);
			if (foundInProcessed != in_processedNodes.end())
				continue;

			float cost = in_curNode->totalScore + connection.GetCost();
			float h = crowDist(*connectedNode, in_targetNode);
			float totalCost = cost + h;
			
			//check if the frontier already contains this node, if it does keep the larger of the two
			auto foundInFrontier = inout_frontierMap.find(connectedNode);
			if(foundInFrontier != inout_frontierMap.end())
			{
				//if we are worse than the node already in the frontier then we should skip further processing
				if(foundInFrontier->second->totalScore < totalCost)
					continue;
				else
					inout_frontierMap.erase(foundInFrontier);
			}

			std::shared_ptr<ScoredNode> newNode = std::make_shared<ScoredNode>(connectedNode, cost, h, static_cast<Direction::Enum>(i), in_curNode);
			inout_frontier.push(newNode);
			inout_frontierMap.insert(std::pair<PathNode*, std::shared_ptr<ScoredNode>>(connectedNode, newNode));
		}
	}
}


//run A* from the closest node to Blinky to the closest node that Pacman is occupying
Direction::Enum aStar(const World& in_ourWorld, PathNode* currentNode, PathNode* targetNode)
{
	std::priority_queue<std::shared_ptr<ScoredNode>, std::function<bool(std::shared_ptr<ScoredNode>, std::shared_ptr<ScoredNode>)>> frontier(NodeComparer);
	std::map<PathNode*, std::shared_ptr<ScoredNode>> processedNodes;
	std::map<PathNode*, std::shared_ptr<ScoredNode>> frontierMap;

	std::shared_ptr<ScoredNode> rootNode = std::make_shared<ScoredNode>(currentNode, 0, crowDist(*currentNode, *targetNode), Direction::Invalid, nullptr);
	frontier.push(rootNode);
	
	while(frontier.size() > 0 && frontier.top()->node != currentNode)
	{
		std::shared_ptr<ScoredNode> topNode = frontier.top();
		frontier.pop();

		auto frontierEntry = frontierMap.find(topNode->node);

		//If we are not in the frontier map then this is just garbage to be collected
		if(frontierEntry == frontierMap.end())
			continue;

		frontierMap.erase(frontierEntry);
		processedNodes.insert(std::pair<PathNode*, std::shared_ptr<ScoredNode>>(topNode->node, topNode));

		Expand(in_ourWorld, topNode, *targetNode, frontier, frontierMap, processedNodes);
	}

	if(frontier.size() > 0)
	{
		ScoredNode* rootConnection = frontier.top()->GetRootConnection();
		if (rootConnection)
			return rootConnection->connectionDirection;
	}

	return Direction::Invalid;
}

Direction::Enum OnBlinkyThink(const Pawn& in_ourPawn, const World& in_ourWorld, float in_deltaTime, float in_totalTime)
{
	static int state;

	std::vector<PathNode*> world;
	in_ourWorld.GetNodes(world);

	switch (in_ourPawn.GetState())
	{
	case 1:
		PathNode* startingTargetNode = in_ourWorld.GetNode(SCATTER_NODE);
		return aStar(in_ourWorld, in_ourWorld.GetNode(in_ourPawn.GetClosestNode()), startingTargetNode); //start to navigate to respective corner 
	case 0:
		//Chase
		//standard mode, attempt to move to the node that PacMan is currently occupying
		//Use A* for this
		return aStar(in_ourWorld, in_ourWorld.GetNode(in_ourPawn.GetClosestNode()), in_ourWorld.GetNode(in_ourWorld.GetPacman().GetClosestNode()));
	case 2:
		//Frightened
		//Pacman has eaten a power cell thingy
		//Attempt to navigate to same corner as scatter
		PathNode* frightTargetNode = in_ourWorld.GetNode(FRIGHTEN_NODE);
		return aStar(in_ourWorld, in_ourWorld.GetNode(in_ourPawn.GetClosestNode()), frightTargetNode); //start to navigate to respective corner
	case 3:
		//Dead
		//Return to the house
		PathNode* houseNode = in_ourWorld.GetNode(HOUSE_NODE);
		return aStar(in_ourWorld, in_ourWorld.GetNode(in_ourPawn.GetClosestNode()), houseNode);
	default:
		return Direction::Up;
	}
}