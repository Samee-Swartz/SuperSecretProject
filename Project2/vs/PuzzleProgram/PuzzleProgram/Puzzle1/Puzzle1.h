#ifndef _PUZZLE_1
#define _PUZZLE_1

#include "Puzzle.h"

#define POPULATION_SIZE 100

class Puzzle1 : public Puzzle {
public:
        Puzzle1();

private:
//        void CheckBestCreature() {
//                for (auto c : population) {
//                        if (c.getScore() > bestChild.getScore())
//                                bestChild = c;
//                }
//        }

        Creature* CreateCreature() const {
                Puzzle1Creature();
        }

        Creature* CreateCreature(const Creature& in_parent1, const Creature& in_parent2) const {
                Puzzle1Creature(in_parent1, in_parent2);
        }
};

#endif

