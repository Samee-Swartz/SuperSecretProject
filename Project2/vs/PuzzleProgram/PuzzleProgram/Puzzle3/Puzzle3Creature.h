#ifndef _PUZZLE_3_CREATURE
#define _PUZZLE_3_CREATURE

#include "../Core/Creature.h"
#include "Puzzle3DNA.h"

class Puzzle3Creature : public Creature {
public:
	Puzzle3Creature() {}

	DNA* CreateDNA() {
		return new Puzzle3DNA();
	}

private:

	DNA* CreateDNA() const;
	float CalculateFitness();
	float CalculateScore();
};

#endif
