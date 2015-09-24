#include "Puzzle3Creature.h"
#include "Puzzle3DNA.h"

DNA* Puzzle3Creature::CreateDNA() const
{
	return new Puzzle3DNA();
}

// Scoring function specified in puzzle instructions
float Puzzle3Creature::CalculateScore() {
	const Puzzle3DNA& dna = static_cast<const Puzzle3DNA&>(GetDNA());
	if (dna.IsDoorOnBottom() && dna.IsLookoutOnTop() && dna.AreWallsInMiddle() &&
	    	dna.VerifyWidths() && dna.VerifyStrength())
		return (float)10 + pow(dna.GetTowerHeight(), 2) - dna.GetTowerCost();
	return 0;
}

float Puzzle3Creature::CalculateFitness() {
	const Puzzle3DNA& dna = static_cast<const Puzzle3DNA&>(GetDNA());
	float fitnessScore = 0;
	if (dna.IsDoorOnBottom())
		fitnessScore += 5;
	if (dna.IsLookoutOnTop())
		fitnessScore += 5;
	if (dna.AreWallsInMiddle())
		fitnessScore += 5;
	if (dna.VerifyWidths())
		fitnessScore += 5;
	if (dna.VerifyStrength())
		fitnessScore += 5;
	return fitnessScore;
}

