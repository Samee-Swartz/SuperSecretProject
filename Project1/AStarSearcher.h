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
		return a->getTotalCost() > b->getTotalCost();
	}
};

class AStarSearcher
{
public:
	static AStarSearcher& create();
	static AStarSearcher& getInstance();

public:
	void computeBestPath(std::vector<AbstractNode*>& out_path);
	void setFinalScore(int m_final) {finalScore = m_final;}
	int getFinalScore() {return finalScore;}
	void addExpandedNode() { expandedNodes++;}
	int getExpandedNodes() {return expandedNodes;}
private:
	AStarSearcher();
private:
	SourceNode* m_rootNode;
	int finalScore;
private:
	//The singleton instance
	static AStarSearcher* m_instance;
	int expandedNodes;
};

#endif
