#ifndef _A_STAR_SEARCHER_
#define _A_STAR_SEARCHER_

#include <vector>
#include "AbstractNode.h"

class SourceNode;

class AStarPriorityQueueComparer
{
public:
	bool operator()(AbstractNode* a, AbstractNode* b)
	{
		return a->getTotalCost() < b->getTotalCost();
	}
};

class AStarSearcher
{
public:
	static AStarSearcher& create();
	static AStarSearcher& getInstance();
	
public:
	void computeBestPath(std::vector<AbstractNode*>& out_path);
private:
	AStarSearcher();
private:
	SourceNode* m_rootNode;
private:
	//The singleton instance
	static AStarSearcher* m_instance;
};

#endif