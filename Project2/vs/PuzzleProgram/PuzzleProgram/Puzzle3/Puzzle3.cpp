#include <string>
#include <time.h>
#include "Puzzle3.h"


Puzzle3::Puzzle3(std::string in_file, int seconds) : m_secondsToRun(seconds){
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
