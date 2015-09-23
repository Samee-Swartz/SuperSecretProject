#ifndef _PUZZLE_3
#define _PUZZLE_3

#include "../Core/Puzzle.h"
#include "Puzzle3Creature.h"

class Puzzle3 : public Puzzle {
public:
	Puzzle3(){}

private:
	Creature* CreateCreature() const { return new Puzzle3Creature(); }
};

#endif
