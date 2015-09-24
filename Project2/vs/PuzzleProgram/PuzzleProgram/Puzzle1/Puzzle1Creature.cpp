#include "Puzzle1Creature.h"
#include "Puzzle1DNA.h"

DNA* Puzzle1Creature::CreateDNA() const {
	return new Puzzle1DNA();
}

// Scoring function specified in puzzle instructions
float Puzzle1Creature::CalculateScore() {
	const Puzzle1DNA& dna = static_cast<const Puzzle1DNA&>(GetDNA());
	return dna.GetSum();
}

float Puzzle1Creature::CalculateFitness() {
    const Puzzle1DNA& dna = static_cast<const Puzzle1DNA&>(GetDNA());
	return dna.GetSum();
}

