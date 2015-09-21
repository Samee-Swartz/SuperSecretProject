#ifndef _PUZZLE_3_CREATURE
#define _PUZZLE_3_CREATURE

#include "Creature.h"

class Puzzle3Creature : public Creature {
public:
	// Used to reproduce creatures from parents
	Puzzle3Creature(const Puzzle3Creature& in_parent1, const Puzzle3Creature& in_parent2);
	// Used to randomly generate first generation of creatures
	Puzzle3Creature(const vector<TowerPiece> in_validDNA);
	// Scoring function specified in puzzle instructions
	float GetScore();

private:
	// Utility function
	float CalculateFitness();
};

#endif
