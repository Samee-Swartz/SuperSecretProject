#include "Puzzle2Creature.h"
#include "Puzzle2DNA.h"


	//Default Puzzle2Creature constructor
	Puzzle2Creature::Puzzle2Creature() { }

	// Scoring function specified in puzzle instructions
	float Puzzle2Creature::CalculateScore() {
		return m_dna.getBin1Val() + m_dna.getBin2Val();
	}

	//utility function
	float Puzzle2Creature::CalculateFitness() {
		return m_dna.getBin1Val() + m_dna.getBin2Val();
	}


