#ifndef _PUZZLE_1_CREATURE
#define _PUZZLE_1_CREATURE

#include "../Core/Creature.h"
#include "Puzzle1DNA.h"

class Puzzle1Creature : public Creature {
private:
    // Utility function
    float CalculateFitness();
	DNA* CreateDNA() const;
	float CalculateScore();
};

#endif

