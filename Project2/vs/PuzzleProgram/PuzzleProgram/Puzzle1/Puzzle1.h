#ifndef _PUZZLE_1
#define _PUZZLE_1

#include "../Core/Puzzle.h"
#include "Puzzle1Creature.h"

class Puzzle1 : public Puzzle {
public:
    Puzzle1();

private:
    Creature* CreateCreature() const { return new Puzzle1Creature();}

	void Setup(const std::string& in_fileName, unsigned int& out_populationSize, unsigned int& out_workerCount);
};

#endif

