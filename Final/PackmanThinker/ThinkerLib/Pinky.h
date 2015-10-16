#pragma once

#include "ThinkerLib.h"
#include <queue>

class PathNode;
struct ScoredNode;
class World;
struct Pawn;

Direction::Enum OnPinkyThink(const Pawn& in_ourPawn, const World& in_ourWorld, float in_deltaTime, float in_totalTime);

void Expand(const World& in_world, ScoredNode in_curNode, std::priority_queue<ScoredNode>& out_frontier);

float CalculateHeuristic(const PathNode* in_node);