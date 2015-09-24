#ifndef _PUZZLE_2_CREATURE
#define _PUZZLE_2_CREATURE

#include "../Core/Creature.h"

class Puzzle2Creature : public Creature {
public:

	//Default Puzzle2Creature constructor
	Puzzle2Creature();


private:
	// Utility function
	float CalculateFitness();
	// Scoring function specified in puzzle instructions
	float CalculateScore();

	DNA* CreateDNA() const;
};

#endif
