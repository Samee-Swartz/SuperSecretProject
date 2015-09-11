#include <cstddef>

#include "AStarSearcher.h"
#include "Nodes.h"
#include "World.h"

bool AStarPriorityQueueComparer(const AbstractNode* const a, const AbstractNode* const b)
{
	return a->getTotalCost() > b->getTotalCost();
}

AbstractNode* goBackToRoot(AbstractNode* startNode)
{
	AbstractNode* last = NULL;
	while(startNode)
	{
		startNode->onExit();
		last = startNode;
		startNode = startNode->getParent();
	}

	return last;
}

void goToChild(AbstractNode* currentNode, AbstractNode* endNode)
{
	AbstractNode* root = goBackToRoot(currentNode);

	std::stack<AbstractNode*> path;

	AbstractNode* node = endNode;

	while(node != NULL)
	{
		path.push(node);
		node = node->getParent();
	}

	while(!path.empty())
	{
		node = path.top();
		path.pop();

		node->onEnter();
	}
}

AStarSearcher::AStarSearcher()
{
	m_rootNode = new SourceNode(0);
}

AStarSearcher& AStarSearcher::create()
{
	if(!m_instance)
		m_instance = new AStarSearcher();
		
	return *m_instance;
}

AStarSearcher& AStarSearcher::getInstance()
{
	return *m_instance;
}

void AStarSearcher::computeBestPath(std::vector<AbstractNode*>& out_path)
{
	std::priority_queue<AbstractNode*, std::vector<AbstractNode*>, decltype(&AStarPriorityQueueComparer)> edgeList(&AStarPriorityQueueComparer);

	AbstractNode* currentNode = m_rootNode;

	while(!currentNode.isEnd())
	{
		currentNode->spawnChildren();
		std::vector<AbstractNode*>* children = currentNode->getChildren();

		//put all children into edge list
		for(std::vector<AbstractNode*>::iterator child : *children)
		{
			edgeList.push(*child);
		}

		AbstractNode* nextBest = edgeList.top();
		edgeList.pop();

		//if we did not select a child node then we have to go back down and up through the tree
		if(nextBest->getParent() != currentNode)
			goToChild(nextBest);
		else
			nextBest->onEnter();

		currentNode = nextBest;
	}

	while(currentNode)
	{
		out_path.insert(0, currentNode);
	}
}