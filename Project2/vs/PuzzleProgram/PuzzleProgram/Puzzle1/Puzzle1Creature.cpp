#include "Puzzle1Creature.h"
#include "Puzzle1DNA.h"


// Used to reproduce creatures from parents
Puzzle1Creature::Puzzle1Creature(const Puzzle1Creature& in_parent1, const Puzzle1Creature& in_parent2) {
        m_dna = Puzzle1DNA(in_parent1, in_parent2);
}

// Used to randomly generate first generation of creatures
Puzzle1Creature::Puzzle3Creature(const vector<int> in_validDNA) {
        m_dna = Puzzle1DNA(in_validDNA);
}

// Scoring function specified in puzzle instructions
float Puzzle1Creature::GetScore() {
	int = sum;

	for(std::vector<int>::iterator j = vector.begin(); j! = vector.end(); ++j)
		sum += *j;

	if(sum <= m_target)
		return sum;
        
	return 0;
}

float Puzzle1Creature::CalculateFitness() {
        int = sum;

        for(std::vector<int>::iterator j = vector.begin(); j! = vector.end(); ++j)
                sum += *j;

        if(sum <= m_target)
                return sum;

        return 0;
}

