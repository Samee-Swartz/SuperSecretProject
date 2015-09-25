#ifndef _PUZZLE_2
#define _PUZZLE_2

#include "../Core/Puzzle.h"
#include "Puzzle2Creature.h"

class Puzzle2 : public Puzzle {
private:
	void Setup(const std::string& in_file, unsigned int& out_populationSize, unsigned int& out_workerCount);

	virtual Creature* CreateCreature() const override;
};

#endif
