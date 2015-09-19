#include <string>
/*
holds vector Creatures - current population
current best offspring found
current best generation #
final generation #
Selection rule - how to choose parents
runs crossover
*/

class Puzzle3Rules : public PuzzleRules {
public:
	Puzzle3Rules(std::string in_file, int seconds) {
		// parse file
		// fills m_validDNA
	}
	void runAlgorithm() {

		// creates initial population
		// while curTime < origTime + seconds
		// 		CheckBestCreature()
		// 		CreateNextGeneration()
	}

private:
	vector<TowerPiece> m_validPieces; // pieces read from the input file
	// check that changing reference changes original
	void MutateCreature(Puzzle3Creature& in_child) {
		// mutation
		// in_child.getPieces()...
	}
	void CorrectCreature(Puzzle3Creature& in_child) {
		// verifies that child is valid, corrects it, if it was invalid
		// in_child.getPieces()
		// check that each piece is only used once
		// check that each piece is in m_validPieces
	}
	void CreateNextGeneration() {
		// takes population, selects parents
		// for each parent set:
		//		Puzzle3Creature(parent1, parent2)
		// 		MutateCreature()
		//		CorrectCreature()
	}
	void CheckBestCreature() {
		for (auto c : population) {
			if (c.getScore() > bestChild.getScore())
				bestChild = c;
		}
	}
};
