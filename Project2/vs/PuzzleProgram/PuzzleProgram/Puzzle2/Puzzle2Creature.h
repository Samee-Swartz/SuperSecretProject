#ifndef _PUZZLE_2_CREATURE
#define _PUZZLE_2_CREATURE

#include "Creature.h"

class Puzzle2Creature : public Creature {
public:
	// Used to reproduce creatures from parents
	Puzzle2Creature(const Puzzle2Creature& in_parent1, const Puzzle2Creature& in_parent2);
	// Used to randomly generate first generation of creatures
	Puzzle2Creature();
	// Scoring function specified in puzzle instructions
	float GetScore();

private:
	// Utility function
	float CalculateFitness();
};

#endif
