#include <cstddef>
#include <iostream>

#include <queue>
#include <functional>

#include "AStarSearcher.h"
#include "Nodes.h"
#include "World.h"

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

AStarSearcher* AStarSearcher::m_instance = NULL;

AStarSearcher::AStarSearcher()
{
	m_rootNode = new SourceNode(World::getInstance().getStartState());
}

AStarSearcher& AStarSearcher::create()
{
	if(!m_instance) {
		m_instance = new AStarSearcher();
	}

	return *m_instance;
}

AStarSearcher& AStarSearcher::getInstance()
{
	return *m_instance;
}

void AStarSearcher::computeBestPath(std::vector<AbstractNode*>& out_path)
{
	std::priority_queue<AbstractNode*, std::vector<AbstractNode*>, AStarPriorityQueueComparer> edgeList;

	AbstractNode* currentNode = m_rootNode;

	std::cout << "starting best path" << std::endl;

	while(!currentNode->isEnd())
	{
		std::cout << "finding ";
		currentNode->spawnChildren();
		const std::vector<AbstractNode*>& children = currentNode->getChildren();

		for(int i = 0; i < children.size(); i++)
		{
			edgeList.push(children[i]);
		}

		AbstractNode* nextBest = edgeList.top();
		edgeList.pop();

		//if we did not select a child node then we have to go back down and up through the tree
		if(nextBest->getParent() != currentNode)
			goToChild(currentNode, nextBest);
		else
			nextBest->onEnter();

		currentNode = nextBest;
		currentNode->onEnter();
	}

	while(currentNode)
	{
		out_path.insert(out_path.begin(), currentNode);
		currentNode = currentNode->getParent();
	}
}
