#include <string>
#include <fstream>
#include <time.h>
#include "Puzzle3.h"

#define WORKER_COUNT 10
#define POPULATION_SIZE 1000

void Puzzle3::Setup(const std::string& in_fileName, unsigned int& out_populationSize, unsigned int& out_workerCount) {
	std::ifstream givenWorld(in_fileName.c_str());
	std::vector<TowerPiece> validPieces;

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
		iss >> t.m_width;
		iss >> t.m_strength;
		iss >> t.m_cost;

		if ((s.compare("Door") == 0) || (s.compare("door") == 0))
			t.m_type = DOOR;
		else if ((s.compare("Wall") == 0) || (s.compare("wall") == 0))
			t.m_type = WALL;
		else if ((s.compare("Lookout") == 0) || (s.compare("lookout") == 0))
			t.m_type = LOOKOUT;
		else
			std::cout << "input file has bad piece type" << std::endl;

		validPieces.push_back(t);
	}

	Puzzle3DNA::SetValidPieces(validPieces);
	out_populationSize = POPULATION_SIZE;
	out_workerCount = WORKER_COUNT;
}
