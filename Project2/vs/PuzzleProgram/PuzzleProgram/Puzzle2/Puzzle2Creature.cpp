#include "Puzzle2Creature.h"
#include "Puzzle2DNA.h"

	// Used to reproduce creatures from parents
	Puzzle2Creature::Puzzle2Creature(const Puzzle2Creature& in_parent1, const Puzzle2Creature& in_parent2) {
		m_dna = DNA(in_parent1, in_parent2);
	}

	// Used to randomly generate first generation of creatures
	Puzzle2Creature::Puzzle2Creature() { 
		m_dna = Puzzle2DNA();
	}

	// Scoring function specified in puzzle instructions
	float Puzzle2Creature::GetScore() {
		return m_dna.getBin1Val() + m_dna.getBin2Val();
	}

	//utility function
	float Puzzle2Creature::CalculateFitness() {
		return m_dna.getBin1Val() + m_dna.getBin2Val();
	}


