#ifndef _PUZZLE_2
#define _PUZZLE_2

#include "../Core/Puzzle.h"
#include "Puzzle2Creature.h"

class Puzzle2 : public Puzzle {
public:
	Puzzle2();

private:
	void Setup(std::string in_file, unsigned int& out_populationSize, unsigned int& out_workerCount);

	Creature* CreateCreature() const { return new Puzzle2Creature(); }
};

#endif
