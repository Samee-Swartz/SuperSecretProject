#ifndef _PUZZLE_2
#define _PUZZLE_2

#include "Puzzle.h"

class Puzzle2 : public Puzzle {
public:
	Puzzle2();

private:
	Creature* CreateCreature() const { Puzzle2Creature(); }

	Creature* CreateCreature(const Creature& in_parent1, const Creature& in_parent2) const {
		Puzzle2Creature(in_parent1, in_parent2);
	}
};

#endif
