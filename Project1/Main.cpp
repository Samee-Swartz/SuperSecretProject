#include <iostream>

#include "AbstractNode.h"
#include "World.h"
#include "AStarSearcher.h"

void usage() {
	std::cout << "This program runs a* search using a given heuristic on a world. "
	<< "If no world is given, one will be randomly generated." << std::endl;
	std::cout << "./program <file containing world> <heuristic value 1-6>  OR" << std::endl;
	std::cout << "./program <difficulty level for world 1-5> <heuristic value 1-6>" << std::endl;
	exit(1);
}

/*Your program should be called astar should accept a command line input of a filename,
and which heuristic should be used (1 through 6).  The file will be a tab-delimited file,
meeting the specifications given above (see the included sample maze).
*/
int main(int argc, char** argv)
{
	if (argc == 3) {
		int h = atoi(argv[2]);
		int d = atoi(argv[1]);
		if (h == 0) {
			usage();
		}
		if (d == 0) { // the first argument is the file
			World::createWorldFrom(argv[1]).setHeuristic(h);
		} else { // first argument is difficulty level
			World::generateWorld(d).setHeuristic(h);
		}
	} else {
		usage();
	}

	std::cout << "start state: " <<
		World::getInstance().getStartState().getRobotPosition().x << "  "
		<< World::getInstance().getStartState().getRobotPosition().y << std::endl;

	std::vector<AbstractNode*> bestPath;
	AStarSearcher searcher = AStarSearcher::create();
	searcher.computeBestPath(bestPath);

	std::cout << "score: " << searcher.getFinalScore() << std::endl;
	std::cout << "number of actions: " << bestPath.size() << std::endl;
	// TODO: get this vv
	std::cout << "number of nodes expanded: " << searcher.getExpandedNodes() << std::endl;
	std::cout << "actions taken:" << std::endl;

	for(int i = 0; i < bestPath.size(); i++)
	{
		std::cout << bestPath[i]->getNodeType();
		if (i != bestPath.size() - 1)
		{
			std::cout << ", ";
			if (i % 7 == 0)
				std::cout << std::endl;
		}
		//print out nodes
	}
	std::cout << std::endl;

#if _WIN32
	system("pause");
#endif
}
