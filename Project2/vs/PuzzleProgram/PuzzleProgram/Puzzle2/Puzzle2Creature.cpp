#include "Puzzle2Creature.h"
#include "Puzzle2DNA.h"


//Default Puzzle2Creature constructor
Puzzle2Creature::Puzzle2Creature() { }

// Scoring function specified in puzzle instructions
float Puzzle2Creature::CalculateScore() {
	const Puzzle2DNA& dna = static_cast<const Puzzle2DNA&>(GetDNA());
	return (dna.getBin1Val() + dna.getBin2Val()) / 2.f;
}

//utility function
float Puzzle2Creature::CalculateFitness() {
	const Puzzle2DNA& dna = static_cast<const Puzzle2DNA&>(GetDNA());
	return (dna.getBin1Val() + dna.getBin2Val()) / 2.f;
}

DNA* Puzzle2Creature::CreateDNA() const {
	return new Puzzle2DNA();
}
