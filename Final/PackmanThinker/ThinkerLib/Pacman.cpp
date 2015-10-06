#include "Pacman.h"
#include "Direction.h"
#include "Pawn.h"
#include "World.h"

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

Direction::Enum OnPacmanThink(const Pawn& in_ourPawn, const World& in_ourWorld, float in_deltaTime, float in_totalTime) {
	int itemScore;
	float north = 0;
	float south = 0;
	float east = 0;
	float west = 0;

	std::vector<PointObj*> points;
	in_ourWorld.GetPointObjects(points);

	for (auto pt : points) {
		switch (pt.GetType()) {
		case PointObj::Type::SuperDot:
			itemScore -= (in_ourPawn.GetPosition() - pt.GetPosition()).MagnitudeSqr();
			break;
		case PointObj::Type::Fruit:
			itemScore -= (in_ourPawn.GetPosition() - pt.GetPosition()).MagnitudeSqr();
		case PointObj::Type::Dot:

		default:
			break;
		}
		// if regular dot
		//		itemScore = -distance to dot
		// if super dot
		//		itemScore = -distance to dot/2
		// if ghost
		// 		itemScore = -5

		switch (WhichDirection(curPosition, item.GetPosition())) {
		case Direction::Up:
			north += itemScore;
			break;
		case Direction::Right:
			east += itemScore;
			break;
		case Direction::Down:
			south += itemScore;
			break;
		case Direction::Left:
			west += itemScore;
			break;
		default:
			break;
		}
	}
	return // highest scoring direction;
}

Direction::Enum WhichDirection(Vector2 in_pacmanPos, Vector2 in_itemPos) {
	if (in_itemPos.y >= (in_pacmanPos.x + in_pacmanPos.y - in_itemPos.x)) {
		if (in_itemPos.y >= (in_pacmanPos.x + in_pacmanPos.y + in_itemPos.x))
			return Direction::Up;
		return Direction::Right;
	}
	if (in_itemPos.y >= (in_pacmanPos.x + in_pacmanPos.y + in_itemPos.x))
		return Direction::Left;
	return Direction::Down;
}
