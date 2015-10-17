#pragma once
#include "Direction.h"


struct Pawn;
class World;

namespace Pacman3
{
	Direction::Enum OnThinkPacman3(const Pawn& in_ourPawn, const World& in_ourWorld, float in_deltaTime, float in_totalTime);
}