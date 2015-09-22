#ifndef _PUZZLE_1
#define _PUZZLE_1

#include "Puzzle.h"

#define POPULATION_SIZE 100

class Puzzle1 : public Puzzle {
public:
        Puzzle1(std::string in_file, int seconds);

private:
        vector<int> m_validDNA;
        vector<Puzzle1Creature> m_population;
        time_t m_initTime;
        int m_secondsToRun;

        void CheckBestCreature() {
                for (auto c : population) {
                        if (c.getScore() > bestChild.getScore())
                                bestChild = c;
                }
        }

        Creature* CreateCreature() const {
                Puzzle1Creature(m_validDNA);
        }

        Creature* CreateCreature(const Creature& in_parent1, const Creature& in_parent2) const {
                Puzzle1Creature(in_parent1, in_parent2);
        }
};

#endif

