#ifndef _A_STAR_SEARCHER_
#define _A_STAR_SEARCHER_

#include <vector>

class AbstractNode;

class AStarSearcher
{
public:
	static AStarSearcher& create();
	static AStarSearcher& getInstance();
	
public:
	void computeBestPath(std::vector<AbstractNode>& out_path);
private:
	AStarSearcher();
private:
	StartNode* m_rootNode;
private:
	//The singleton instance
	static AStarSearcher* m_instance;
};

#endif