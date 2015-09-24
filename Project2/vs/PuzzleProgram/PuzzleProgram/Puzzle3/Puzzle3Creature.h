#ifndef _PUZZLE_3_CREATURE
#define _PUZZLE_3_CREATURE

#include "../Core/Creature.h"
#include "Puzzle3DNA.h"

class Puzzle3Creature : public Creature {
private:

	DNA* CreateDNA() const;
	float CalculateFitness();
	float CalculateScore();
};

#endif
