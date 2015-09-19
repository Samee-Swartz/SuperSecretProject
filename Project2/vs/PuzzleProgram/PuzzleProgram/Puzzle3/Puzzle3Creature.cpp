#include "Creature.h"
#include "DNA.h"

class Puzzle3Creature : public Creature {
public:
	// Used to reproduce creatures from parents
	Puzzle3Creature(const Puzzle3Creature& in_parent1, const Puzzle3Creature& in_parent2) {
		m_dna = DNA(in_parent1, in_parent2);
	}

	// Used to randomly generate first generation of creatures
	Puzzle3Creature(const vector<TowerPiece> in_validDNA) {
		m_dna = DNA(in_validDNA);
	}

	// Scoring function specified in puzzle instructions
	float GetScore() {
		if (m_dna.IsDoorOnBottom() && m_dna.IsLookoutOnTop() && m_dna.AreWallsInMiddle() &&
		    	m_dna.VerifyWidths() && m_dna.VerifyStrength())
			return (float)10 + pow(m_dna.GetTowerHeight(), 2) - m_dna.GetTowerCost();
		return 0;
	}

private:
	// Utility function
	// TODO: MAKE THIS BETTER
	float CalculateFitness() {
		if (m_dna.IsDoorOnBottom() && m_dna.IsLookoutOnTop() && m_dna.AreWallsInMiddle() &&
		    	m_dna.VerifyWidths() && m_dna.VerifyStrength())
			return (float)10 + pow(m_dna.GetTowerHeight(), 2) - m_dna.GetTowerCost();
		return 0;
	}

};
