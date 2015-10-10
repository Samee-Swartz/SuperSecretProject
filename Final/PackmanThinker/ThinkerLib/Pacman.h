#pragma once
#include "World.h"
#include "Pawn.h"
#include "Direction.h"

Direction::Enum OnPacmanThink(const Pawn& in_ourPawn, const World& in_ourWorld,
                              float in_deltaTime, float in_totalTime);
