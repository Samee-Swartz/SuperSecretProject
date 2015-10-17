#include "Pacman3.h"
#include "World.h"
#include <memory>
#include <queue>

namespace Pacman3
{
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
			if (prev.get())
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

	float CalculateGhostEffect(const PathNode& in_node, const std::vector<const Pawn* const> in_ghosts)
	{
		float score = 0;
		for(auto ghost : in_ghosts)
		{
			score += (ghost->GetPosition() - in_node.GetPosition()).MagnitudeSqr() / 1.0f;
			if (ghost->GetClosestNode() == in_node.GetId())
				score += 1000;

			//When blue boost our want to go towards the ghost
			if (ghost->GetState() == 2)
				score = -score;
		}

		return score;
	}

	float CalculatePointBonus(const PathNode& in_node)
	{
		const PointObj* const pointObj = in_node.GetObject();
		if (!pointObj)
			return 1;

		return -pointObj->GetWorth();
	}

	//Calculates the straight line distance between the currently node in examination and the target node
	float huristic(const PathNode& aNode, const PathNode& bNode)
	{
		return (bNode.GetPosition() - aNode.GetPosition()).Magnitude();
	}

	bool NodeComparer(std::shared_ptr<ScoredNode> in_a, std::shared_ptr<ScoredNode> in_b)
	{
		return in_a->totalScore > in_b->totalScore;
	}

	// expands the current node in all four directions. Adds valid expansions to the frontier list
	void Expand(const World& in_world,
	            std::shared_ptr<ScoredNode> in_curNode,
	            const PathNode& in_targetNode,
				std::vector<const Pawn* const> in_ghosts,
	            std::priority_queue<std::shared_ptr<ScoredNode>, std::vector<std::shared_ptr<ScoredNode>>, std::function<bool(std::shared_ptr<ScoredNode>, std::shared_ptr<ScoredNode>)>>& inout_frontier,
	            std::map<PathNode*, std::shared_ptr<ScoredNode>>& inout_frontierMap,
	            const std::map<PathNode*, std::shared_ptr<ScoredNode>>& in_processedNodes)
	{
		for (int i = 0; i < 4; i++)
		{
			const PathNodeConnection& connection = in_curNode->node->GetConnection(static_cast<Direction::Enum>(i));
			if (connection.IsValid())
			{
				//Gather the information on this next connection
				PathNode* connectedNode = in_world.GetNode(connection.GetOtherNodeId());

				//If the node has already been processed then by definition of A* we must be back tracking and we can ignore this connection
				auto foundInProcessed = in_processedNodes.find(connectedNode);
				if (foundInProcessed != in_processedNodes.end())
					continue;

				float cost = in_curNode->totalScore + connection.GetCost();
				float h = huristic(*connectedNode, in_targetNode) + CalculateGhostEffect(*connectedNode, in_ghosts) + CalculatePointBonus(*connectedNode);
				float totalCost = cost + h;

				//check if the frontier already contains this node, if it does keep the larger of the two
				auto foundInFrontier = inout_frontierMap.find(connectedNode);
				if (foundInFrontier != inout_frontierMap.end())
				{
					//if we are worse than the node already in the frontier then we should skip further processing
					if (foundInFrontier->second->totalScore < totalCost)
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
		std::priority_queue<std::shared_ptr<ScoredNode>, std::vector<std::shared_ptr<ScoredNode>>, std::function<bool(std::shared_ptr<ScoredNode>, std::shared_ptr<ScoredNode>)>> frontier(NodeComparer);
		std::map<PathNode*, std::shared_ptr<ScoredNode>> processedNodes;
		std::map<PathNode*, std::shared_ptr<ScoredNode>> frontierMap;

		std::vector<const Pawn* const> ghosts;
		ghosts.push_back(&in_ourWorld.GetBlinky());
		ghosts.push_back(&in_ourWorld.GetInky());
		ghosts.push_back(&in_ourWorld.GetPinky());
		ghosts.push_back(&in_ourWorld.GetClyde());

		std::shared_ptr<ScoredNode> rootNode = std::make_shared<ScoredNode>(currentNode, 0.f, huristic(*currentNode, *targetNode), Direction::Invalid, nullptr);
		frontier.push(rootNode);
		frontierMap.insert(std::pair<PathNode*, std::shared_ptr<ScoredNode>>(currentNode, rootNode));

		while (frontier.size() > 0 && frontier.top()->node != targetNode)
		{
			std::shared_ptr<ScoredNode> topNode = frontier.top();
			frontier.pop();

			auto frontierEntry = frontierMap.find(topNode->node);

			//If we are not in the frontier map then this is just garbage to be collected
			if (frontierEntry == frontierMap.end())
				continue;

			frontierMap.erase(frontierEntry);
			processedNodes.insert(std::pair<PathNode*, std::shared_ptr<ScoredNode>>(topNode->node, topNode));

			Expand(in_ourWorld, topNode, *targetNode, ghosts, frontier, frontierMap, processedNodes);
		}

		if (frontier.size() > 0)
		{
			ScoredNode* rootConnection = frontier.top()->GetRootConnection();
			if (rootConnection)
				return rootConnection->connectionDirection;
		}

		return Direction::Invalid;
	}


	Direction::Enum OnThinkPacman3(const Pawn& in_ourPawn, const World& in_ourWorld, float in_deltaTime, float in_totalTime)
	{
		std::vector<PointObj*> pointObjects;
		in_ourWorld.GetPointObjects(pointObjects);

		Vector2 avgBestLocation = Vector2(0, 0);

		for(auto obj : pointObjects)
		{
			avgBestLocation = avgBestLocation + obj->GetPosition();
		}

		PointObj* closestObject = nullptr;
		float closestSqr = FLT_MAX;
		for (auto obj : pointObjects)
		{
			float distanceSqr = (obj->GetPosition() - avgBestLocation).MagnitudeSqr();
			if(distanceSqr < closestSqr)
			{
				closestSqr = distanceSqr;
				closestObject = obj;
			}
		}

		if (!closestObject)
			return Direction::Invalid;

		PathNode* start = in_ourWorld.GetNode(in_ourPawn.GetClosestNode());
		PathNode* destination = in_ourWorld.GetNode(closestObject->GetNode());

		return aStar(in_ourWorld, start, destination);
	}
}