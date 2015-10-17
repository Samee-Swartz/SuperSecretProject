#pragma once

#include "ThinkerLib.h"
#include <queue>

class PathNode;
struct ScoredNode;
class World;
struct Pawn;

namespace Clyde
{
	Direction::Enum OnClydeThink(const Pawn& in_ourPawn, const World& in_ourWorld, float in_deltaTime, float in_totalTime);
}