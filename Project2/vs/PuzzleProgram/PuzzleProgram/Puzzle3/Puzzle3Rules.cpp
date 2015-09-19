#include <string>
#include <time.h>

/*
holds vector Creatures - current population
current best offspring found
current best generation #
final generation #
Selection rule - how to choose parents
runs crossover
*/
#define POPULATION_SIZE 100

class Puzzle3Rules : public PuzzleRules {
public:
	Puzzle3Rules(std::string in_file, int seconds) : m_secondsToRun(seconds){
		std::ifstream givenWorld(in_file.c_str());
		if (!givenWorld) { // problem...
			std::cout << "Given input file is invalid." << std::endl;
			// TODO: handle somehow?
			return;
		}
		std::string line;
		while (std::getline(givenWorld, line)) {
			TowerPiece t;
			std::string s;
			std::istringstream iss(line);
			iss >> s;
			iss >> t.width;
			iss >> t.strength;
			iss >> t.cost;

			if ((s.compare("Door") == 0) || (s.compare("door") == 0))
				t.type = DOOR;
			else if ((s.compare("Wall") == 0) || (s.compare("wall") == 0))
				t.type = WALL;
			else if ((s.compare("Lookout") == 0) || (s.compare("lookout") == 0))
				t.type = LOOKOUT;
			else
				std::cout << "input file has bad piece type"

			m_validDNA.push_back(t);
		}
	}
	void runAlgorithm() {
		time(&m_initTime);
		time_t curTime;
		time(&curTime);
		while (int count = 0; count < 100; count++) {
			population.push_back(Puzzle3Creature(m_validDNA));
		}

		// while we still have time to run
		while (difftime(m_initTime, curTime) < (float) m_secondsToRun) {
			CheckBestCreature();
			CreateNextGeneration();
			time(&curTime);
		}
	}

private:
	vector<TowerPiece> m_validDNA;
	vector<Puzzle3Creature> m_population;
	time_t m_initTime;
	int m_secondsToRun;
	void CreateNextGeneration() {
		// takes population, selects parents
		// for each parent set:
		//		Puzzle3Creature(parent1, parent2)
	}
	void CheckBestCreature() {
		for (auto c : population) {
			if (c.getScore() > bestChild.getScore())
				bestChild = c;
		}
	}
};
