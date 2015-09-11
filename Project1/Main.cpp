#include <iostream>

#include "AbstractNode.h"
#include "World.h"
#include "AStarSearcher.h"

int main(int argc, char** argv)
{
	World::generateWorld();

	std::vector<AbstractNode*> bestPath;
	AStarSearcher& searcher = AStarSearcher::create();
	searcher.computeBestPath(bestPath);

	for(int i = 0; i < bestPath.size(); i++)
	{
		//print out nodes
	}
}