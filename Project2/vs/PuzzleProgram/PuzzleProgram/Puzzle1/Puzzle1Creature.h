#ifndef _PUZZLE_1_CREATURE
#define _PUZZLE_1_CREATURE

#include ../Core/"Creature.h"
#include "Puzzle1DNA.h"

class Puzzle1Creature : public Creature {
public:
        // Used to reproduce creatures from parents
//        Puzzle1Creature(const Puzzle1Creature& in_parent1, const Puzzle1Creature& in_parent2);

        // Scoring function specified in puzzle instructions
  //      float GetScore();

private:
        // Utility function
        float CalculateFitness();

	DNA* CreateDNA() const;
	float CalculateScore();

//	int m_target;
};

#endif

