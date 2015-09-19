#include "Creature.h"
#include "DNA.h"
/*
Class Creature
Created from two (parent) Creature
Constructor does crossover from parents
Empty constructor for first generation (randomly generated)
Utility function - overridden for each puzzle
Contains DNA - specific to the puzzle
*/

/*
1. Towers must have a door as their bottom-most piece.
2. The top piece in a tower must be a lookout.
3. Pieces between the top and bottom of a tower, if any, must be composted of wall segments.
4. A piece in a tower can, at most, be as wide as the piece below it.
5. A piece in a tower can support its strength value in pieces placed above it.
*/


class Puzzle3Creature : public Creature {
public:
	Puzzle3Creature(Puzzle3Creature& in_parent1, Puzzle3Creature& in_parent2) {
		// crossover from parents
	}
	Puzzle3Creature() {
		 // used to randomly generate first generation
	}

	float GetScore() {
		// Calculate puzzle's scoring mechanism
	}

private:
	float CalculateFitness() {
		// Utility function
	}
	bool DoorOnBottom() {
		m_dna.GetPieces().front()
		...
	}
	bool LookoutOnTop() {
		m_dna.GetPieces().back()
		...
	}
	bool WallsInMiddle() {
		m_dna.GetPieces()
		...
	}
	bool VerifyWidths() {
		vector<TowerPiece> tower = m_dna.GetPieces();
		int cur_width = tower.front().m_width;
		for (auto p : tower) {
			if (p.m_width < cur_width)
				cur_width = p.m_width;
			else if (p.m_width > cur_width) // unstable tower!
				return false;
		}
		return true;
	}
	bool VerifyStrength() {
		m_dna.GetPieces()
	}
};
