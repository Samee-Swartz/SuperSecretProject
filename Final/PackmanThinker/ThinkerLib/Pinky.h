#pragma once

#include "ThinkerLib.h"

class World;
struct Pawn;

Direction::Enum OnPinkyThink(const Pawn& in_ourPawn, const World& in_ourWorld, float in_deltaTime, float in_totalTime);