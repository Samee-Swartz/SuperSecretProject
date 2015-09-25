#include "Puzzle2.h"
#include <fstream>
#include "Puzzle2DNA.h"
/*
holds vector Creatures - current population
current best offspring found
current best generation #
final generation #
Selection rule - how to choose parents
runs crossover
*/
#define WORKER_COUNT 1
#define POPULATION_SIZE 1000

void Puzzle2::Setup(const std::string& in_fileName, unsigned int& out_populationSize, unsigned int& out_workerCount){
	std::ifstream givenWorld(in_fileName.c_str());
	std::vector<int> validPieces;

	if(!givenWorld){
		std::cout << "Given input file is invalid." << std::endl;
	}

	std::string line;

	int temp;
	while(std::getline(givenWorld, line)){
		temp = atoi(line.c_str());

		if(temp > 10 || temp < -10){
			std::cout << "Invalid value for Puzzle2 validPieces:" << temp << std::endl;
		}

		validPieces.push_back(temp);
	}

	Puzzle2DNA::SetValidPieces(validPieces);
	out_populationSize = POPULATION_SIZE;
	out_workerCount = WORKER_COUNT;
}

Creature* Puzzle2::CreateCreature() const
{
	return new Puzzle2Creature();
}