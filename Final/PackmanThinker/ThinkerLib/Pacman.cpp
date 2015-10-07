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
// A*

//
Direction::Enum OnPacmanThink(const Pawn& in_ourPawn, const World& in_ourWorld, float in_deltaTime, float in_totalTime) {
	PathNode* curNode = in_ourWorld.GetNode(in_ourPawn->GetClosestNode());

}
