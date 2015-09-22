#ifndef _PUZZLE_3
#define _PUZZLE_3

#include "Puzzle.h"

class Puzzle3 : public Puzzle {
public:
	Puzzle3();

private:
	// void CheckBestCreature() {
	// 	for (auto c : population) {
	// 		if (c.getScore() > bestChild.getScore())
	// 			bestChild = c;
	// 	}
	// }

	Creature* CreateCreature() const { Puzzle3Creature(); }

	Creature* CreateCreature(const Creature& in_parent1, const Creature& in_parent2) const {
		Puzzle3Creature(in_parent1, in_parent2);
	}
};

#endif
