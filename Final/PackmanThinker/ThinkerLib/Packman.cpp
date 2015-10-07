#include "Packman.h"
#include "Direction.h"

/*
enum Enum
		{
			Dot,
			SuperDot,
			Fruit
		};
		*/


// A* with goal of most points - heuristic is distance from ghosts

// google pacman strategies
// A*

//

Vector2 OnPacmanThink(const Pawn& in_ourPawn, const World& in_ourWorld, float in_deltaTime, float in_totalTime) {
	std::vector<PathNode*> frontier; // priority queue
	PathNode* curNode = in_ourWorld.GetNode(in_ourPawn->GetClosestNode());


	for (int count =0; count < 15; count++) {

	}
	// int itemScore;
	// float north = 0;
	// float south = 0;
	// float east = 0;
	// float west = 0;

	// std::vector<PointObj*> points;
	// in_ourWorld.GetPointObjects(points);

	// for (auto pt : points) {
	// 	switch (pt.GetType()) {
	// 		case SuperDot:
	// 			itemScore -= (in_ourPawn.GetPosition() - pt.GetPosition()).MagnitudeSqr();
	// 			break;
	// 		case Fruit:
	// 			itemScore -= (in_ourPawn.GetPosition() - pt.GetPosition()).MagnitudeSqr();
	// 		case Dot:

	// 		default:
	// 		break;
	// 	}
	// 	// if regular dot
	// 	//		itemScore = -distance to dot
	// 	// if super dot
	// 	//		itemScore = -distance to dot/2
	// 	// if ghost
	// 	// 		itemScore = -5

	// 	switch (WhichDirection(curPosition, item.GetPosition())) {
	// 		case Up:
	// 		north += itemScore;
	// 		break;
	// 		case Right:
	// 		east += itemScore;
	// 		break;
	// 		case Down:
	// 		south += itemScore;
	// 		break;
	// 		case Left:
	// 		west += itemScore;
	// 		break;
	// 		default:
	// 		break;
	// 	}
	// }
	// return // highest scoring direction;
}

// Direction WhichDirection(Position in_pacmanPos, Position in_itemPos) {
// 	if (in_itemPos.y >= (in_pacmanPos.x + in_pacmanPos.y - in_itemPos.x)) {
// 		if (in_itemPos.y >= (in_pacmanPos.x + in_pacmanPos.y + in_itemPos.x))
// 			return North;
// 		return East
// 	}
// 	if (in_itemPos.y >= (in_pacmanPos.x + in_pacmanPos.y + in_itemPos.x))
// 		return West;
// 	return South;
// }
