#ifndef _PUZZLE_3
#define _PUZZLE_3

#include "Puzzle.h"

#define POPULATION_SIZE 100

class Puzzle3 : public Puzzle {
public:
	Puzzle3(std::string in_file, int seconds);

private:
	vector<TowerPiece> m_validDNA;
	vector<Puzzle3Creature> m_population;
	time_t m_initTime;
	int m_secondsToRun;

	void CheckBestCreature() {
		for (auto c : population) {
			if (c.getScore() > bestChild.getScore())
				bestChild = c;
		}
	}

	Creature* CreateCreature() const {
		Puzzle3Creature(m_validDNA);
	}

	Creature* CreateCreature(const Creature& in_parent1, const Creature& in_parent2) const {
		Puzzle3Creature(in_parent1, in_parent2);
	}
};

#endif
