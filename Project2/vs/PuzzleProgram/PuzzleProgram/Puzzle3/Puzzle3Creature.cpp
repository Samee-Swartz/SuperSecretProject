#include "Puzzle3Creature.h"
#include "Puzzle3DNA.h"

// Scoring function specified in puzzle instructions
float Puzzle3Creature::CalculateScore() {
	if (m_dna.IsDoorOnBottom() && m_dna.IsLookoutOnTop() && m_dna.AreWallsInMiddle() &&
	    	m_dna.VerifyWidths() && m_dna.VerifyStrength())
		return (float)10 + pow(m_dna.GetTowerHeight(), 2) - m_dna.GetTowerCost();
	return 0;
}

float Puzzle3Creature::CalculateFitness() {
	float fitnessScore = 0;
	if (m_dna.IsDoorOnBottom())
		fitnessScore += 5;
	if (m_dna.IsLookoutOnTop())
		fitnessScore += 5;
	if (m_dna.AreWallsInMiddle())
		fitnessScore += 5;
	if (m_dna.VerifyWidths())
		fitnessScore += 5;
	if (m_dna.VerifyStrength())
		fitnessScore += 5;
	return fitnessScore;
}

